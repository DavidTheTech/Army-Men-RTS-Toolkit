#include "LuaInternals.h"
#include "../GameFuncs/system/Log.h"
#include <unordered_map>

//GameState
#define ADDR_6BD4D0 (DWORD*)0x6BD4D0
#define ADDR_728AA0 (DWORD*)0x728AA0
#define VID_ONACTIVATE 0x0041FA90

static bool forcingActive = false;
static HANDLE forcingThread = NULL;
static bool stopForcing = false;
static BYTE originalFirstByte = 0;

struct ForcingState
{
    DWORD forcedBit1;
    DWORD forcedActive;
};

static ForcingState forcedValues;
//end

//Keybinds
struct KeyCombo
{
    bool ctrl = false;
    bool alt = false;
    bool shift = false;
    int vkCode = 0;

    bool operator == (const KeyCombo& other) const
    {
        return ctrl == other.ctrl && alt == other.alt && shift == other.shift && vkCode == other.vkCode;
    }
};

struct KeyComboHash
{
    std::size_t operator()(const KeyCombo& k) const
    {
        return ((k.ctrl ? 1 : 0) << 0) | ((k.alt ? 1 : 0) << 1) | ((k.shift ? 1 : 0) << 2) | (k.vkCode << 3);
    }
};

static std::unordered_map<KeyCombo, int, KeyComboHash> keyBindings;
static bool isProcessingBindings = false;
static bool g_globalKeybinds = true;

static std::unordered_map<std::string, bool*> g_boolProperties;

void RegisterBoolProperty(const char* name, bool* var)
{
    g_boolProperties[name] = var;
}

struct AutoRegister
{
    AutoRegister()
    {
        RegisterBoolProperty("globalBinds", &g_globalKeybinds);
    }
} autoRegister;
//end

//GameState
DWORD WINAPI ForceValuesThread(LPVOID param)
{
    ForcingState* state = (ForcingState*)param;

    while (!stopForcing)
    {
        *ADDR_6BD4D0 = (*ADDR_6BD4D0 & ~2) | state->forcedBit1;
        *ADDR_728AA0 = state->forcedActive;
        Sleep(10);
    }

    return 0;
}

void ApplyPatch(bool enable)
{
    BYTE* target = (BYTE*)VID_ONACTIVATE;
    DWORD oldProtect;

    VirtualProtect(target, 1, PAGE_EXECUTE_READWRITE, &oldProtect);

    if (enable && !forcingActive)
    {
        originalFirstByte = *target;
        *target = 0xC3;
    }
    else if (!enable && forcingActive)
    {
        *target = originalFirstByte;
    }

    VirtualProtect(target, 1, oldProtect, &oldProtect);
}
//end

//Keybinds
static HWND GetGameWindowHandle()
{
    static HWND hwnd = NULL;
    if (!hwnd)
    {
#ifdef USE_MAIN_GETGAMEWINDOW
        hwnd = Main::GetGameWindow();
#else
        hwnd = FindWindowA("Army Men RTS Studio", NULL);
#endif
    }
    return hwnd;
}

static KeyCombo ParseKeyCombo(const std::string& str)
{
    KeyCombo combo;
    std::istringstream iss(str);
    std::string token;
    std::vector<std::string> tokens;
    while (iss >> token)
    {
        tokens.push_back(token);
    }
    if (tokens.empty())
    {
        return combo;
    }

    std::string keyName = tokens.back();
    tokens.pop_back();

    if (keyName.size() == 1 && std::isalpha(keyName[0]))
    {
        combo.vkCode = toupper(keyName[0]);
    }
    else if (keyName.size() == 1 && std::isdigit(keyName[0]))
    {
        combo.vkCode = keyName[0];
    }
    else
    {
        if (keyName == "space") combo.vkCode = VK_SPACE;
        else if (keyName == "enter") combo.vkCode = VK_RETURN;
        else if (keyName == "tab") combo.vkCode = VK_TAB;
        else if (keyName == "escape") combo.vkCode = VK_ESCAPE;
        else if (keyName == "up") combo.vkCode = VK_UP;
        else if (keyName == "down") combo.vkCode = VK_DOWN;
        else if (keyName == "left") combo.vkCode = VK_LEFT;
        else if (keyName == "right") combo.vkCode = VK_RIGHT;
        else if (keyName == "f1") combo.vkCode = VK_F1;
        else if (keyName == "f2") combo.vkCode = VK_F2;
        else if (keyName == "f3") combo.vkCode = VK_F3;
        else if (keyName == "f4") combo.vkCode = VK_F4;
        else if (keyName == "f5") combo.vkCode = VK_F5;
        else if (keyName == "f6") combo.vkCode = VK_F6;
        else if (keyName == "f7") combo.vkCode = VK_F7;
        else if (keyName == "f8") combo.vkCode = VK_F8;
        else if (keyName == "f9") combo.vkCode = VK_F9;
        else if (keyName == "f10") combo.vkCode = VK_F10;
        else if (keyName == "f11") combo.vkCode = VK_F11;
        else if (keyName == "f12") combo.vkCode = VK_F12;
    }

    for (const auto& mod : tokens)
    {
        std::string m = mod;
        for (auto& c : m) c = tolower(c);
        if (m == "ctrl")
        {
            combo.ctrl = true;
        }
        else if (m == "alt")
        {
            combo.alt = true;
        }
        else if (m == "shift")
        {
            combo.shift = true;
        }
    }
    return combo;
}

static bool IsKeyComboPressed(const KeyCombo& combo)
{
    if (combo.ctrl && (GetAsyncKeyState(VK_CONTROL) & 0x8000) == 0) return false;
    if (combo.alt && (GetAsyncKeyState(VK_MENU) & 0x8000) == 0) return false;
    if (combo.shift && (GetAsyncKeyState(VK_SHIFT) & 0x8000) == 0) return false;
    if (combo.vkCode != 0 && (GetAsyncKeyState(combo.vkCode) & 0x8000) == 0) return false;
    return true;
}

void ProcessKeybindings(lua_State* L)
{
    if (!L)
    {
        return;
    }

    static std::unordered_map<KeyCombo, bool, KeyComboHash> lastState;

    for (auto& pair : keyBindings)
    {
        const KeyCombo& combo = pair.first;
        int ref = pair.second;

        bool pressed = false;

        if (g_globalKeybinds)
        {
            pressed = IsKeyComboPressed(combo);
        }
        else
        {
            HWND gameHwnd = GetGameWindowHandle();
            if (GetForegroundWindow() == gameHwnd)
            {
                pressed = IsKeyComboPressed(combo);
            }
        }

        bool wasPressed = lastState[combo];
        if (pressed && !wasPressed)
        {
            lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
            if (lua_isfunction(L, -1))
            {
                if (lua_pcall(L, 0, 0, 0) != LUA_OK)
                {
                    const char* err = lua_tostring(L, -1);
                    Log::Client::Write("[Lua] Keybind error: %s", err);
                    lua_pop(L, 1);
                }
            }
            else
            {
                lua_pop(L, 1);
            }
        }
        lastState[combo] = pressed;
    }
}

int Lua_KeybindsGetProperty(lua_State* L)
{
    const char* key = luaL_checkstring(L, 2);
    auto it = g_boolProperties.find(key);
    if (it != g_boolProperties.end())
    {
        lua_pushboolean(L, *(it->second));
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

int Lua_KeybindsSetProperty(lua_State* L)
{
    const char* key = luaL_checkstring(L, 2);
    bool value = lua_toboolean(L, 3) != 0;
    auto it = g_boolProperties.find(key);
    if (it != g_boolProperties.end())
    {
        *(it->second) = value;
    }
    return 0;
}
//end

//GameState
int Lua_InternalsToggleGameUpdates(lua_State* L)
{
    if (!forcingActive)
    {
        forcedValues.forcedBit1 = 2;
        forcedValues.forcedActive = 1;

        *ADDR_6BD4D0 = (*ADDR_6BD4D0 & ~2) | forcedValues.forcedBit1;
        *ADDR_728AA0 = forcedValues.forcedActive;

        ApplyPatch(true);

        stopForcing = false;
        forcingThread = CreateThread(NULL, 0, ForceValuesThread, &forcedValues, 0, NULL);
        forcingActive = true;

        lua_pushstring(L, "Game updates ENABLED");
    }
    else
    {
        stopForcing = true;
        WaitForSingleObject(forcingThread, 100);
        CloseHandle(forcingThread);
        forcingThread = NULL;

        ApplyPatch(false);

        *ADDR_6BD4D0 = (*ADDR_6BD4D0 & ~2) | 0;
        *ADDR_728AA0 = 0;

        forcingActive = false;

        lua_pushstring(L, "Game updates DISABLED");
    }

    return 1;
}

int Lua_InternalsGetGameState(lua_State* L)
{
    DWORD val_dword = *ADDR_6BD4D0;
    DWORD val_active = *ADDR_728AA0;

    bool bit1Set = (val_dword & 2) != 0;
    bool isActive = (val_active == 1);
    bool isUpdating = bit1Set && isActive;

    lua_pushboolean(L, isUpdating);
    lua_pushboolean(L, bit1Set);
    lua_pushboolean(L, isActive);

    return 3;
}

int Lua_InternalsIsGameUpdating(lua_State* L)
{
    DWORD val_dword = *ADDR_6BD4D0;
    DWORD val_active = *ADDR_728AA0;

    bool isUpdating = ((val_dword & 2) != 0) && (val_active == 1);

    lua_pushboolean(L, isUpdating);
    return 1;
}
//end

//Keybinds
int Lua_InternalsBind(lua_State* L)
{
    const char* comboStr = luaL_checkstring(L, 1);
    luaL_checktype(L, 2, LUA_TFUNCTION);

    KeyCombo combo = ParseKeyCombo(comboStr);
    if (combo.vkCode == 0 && comboStr[0] != '\0')
    {
        luaL_error(L, "Invalid key name in combo: %s", comboStr);
        return 0;
    }

    auto it = keyBindings.find(combo);
    if (it != keyBindings.end())
    {
        luaL_unref(L, LUA_REGISTRYINDEX, it->second);
        keyBindings.erase(it);
    }

    lua_pushvalue(L, 2);
    int ref = luaL_ref(L, LUA_REGISTRYINDEX);
    keyBindings[combo] = ref;
    printf("done\n");
    return 0;
}

int Lua_InternalsUnbind(lua_State* L)
{
    const char* comboStr = luaL_checkstring(L, 1);
    KeyCombo combo = ParseKeyCombo(comboStr);
    auto it = keyBindings.find(combo);
    if (it != keyBindings.end())
    {
        luaL_unref(L, LUA_REGISTRYINDEX, it->second);
        keyBindings.erase(it);
    }
    return 0;
}
//end