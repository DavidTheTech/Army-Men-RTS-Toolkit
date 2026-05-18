#include "LuaEngine.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstring>
#include "..\GameFuncs\system\Log.h"
#include "..\GameFuncs\util\VarSys.h"
#include "..\GameFuncs\util\Console.h"
#include "..\GameFuncs\graphics\Terrain.h"
#include "..\GameFuncs\interface\IFace_messagebox.h"
#include "..\GameFuncs\interface\messagebox_event.h"

#include "LuaMapObject.h"
#include "LuaMapObjManager.h"
#include "LuaInternals.h"
#include "LuaSquadManager.h"
#include "LuaFun.h"
#include "../GameFuncs/coregame/TerrainData.h"

MapObjManager manager;
HANDLE LuaLoopHandle = NULL;
lua_State* g_luaState = nullptr;

// Global map for button text -> Lua reference (used by IFace callbacks)
static std::unordered_map<std::string, int> g_buttonCallbacks;

LuaEngine::LuaEngine() : L(nullptr)
{
}

LuaEngine::~LuaEngine()
{
    if (L)
    {
        lua_close(L);
        L = nullptr;
    }
}

bool LuaEngine::Initialize()
{
    L = luaL_newstate();
    if (!L)
    {
        return false;
    }
    g_luaState = L;

    luaL_openlibs(L);
    RegisterFunctions();

    return true;
}

bool LuaEngine::LoadScript(const std::string& path)
{
    m_scriptPath = path;

    HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        GetFileTime(hFile, NULL, NULL, &m_lastModTime);
        CloseHandle(hFile);
    }
    else
    {
        memset(&m_lastModTime, 0, sizeof(FILETIME));
    }

    if (luaL_dofile(L, path.c_str()) != LUA_OK)
    {
        Log::Client::Write("[LUA ENGINE]: LoadScript Error %s", lua_tostring(L, -1));
        lua_pop(L, 1);

        lua_settop(L, 0);
        return false;
    }

    return true;
}

bool LuaEngine::CheckAndReload()
{
    if (m_scriptPath.empty())
    {
        return false;
    }

    HANDLE hFile = CreateFileA(m_scriptPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    FILETIME currentModTime;
    GetFileTime(hFile, NULL, NULL, &currentModTime);
    CloseHandle(hFile);

    if (CompareFileTime(&currentModTime, &m_lastModTime) != 0)
    {
        Log::Client::Write("[LUA ENGINE]: Reloading %s", m_scriptPath.c_str());
        return LoadScript(m_scriptPath);
    }

    return false;
}

void LuaEngine::LuaLoop()
{
    if (L)
    {
        ProcessKeybindings(L);
    }
}

bool LuaEngine::ExecuteString(const std::string& code)
{
    if (luaL_dostring(L, code.c_str()) != LUA_OK)
    {
        std::cerr << "[Lua] " << lua_tostring(L, -1) << std::endl;
        Log::Client::Write("[LUA ENGINE]: ExecuteString Error %s ", lua_tostring(L, -1));
        lua_pop(L, 1);
        return false;
    }
    return true;
}

void LuaEngine::SetVariable(const std::string& name, int value)
{
    lua_pushinteger(L, value);
    lua_setglobal(L, name.c_str());
}

static void LuaButtonCallback(unsigned int /*event*/, unsigned int context)
{
    const char* buttonText = reinterpret_cast<const char*>(context);
    if (!buttonText)
    {
        return;
    }

    Log::Client::Write("[LUA] Button clicked: %s", buttonText);

    auto it = g_buttonCallbacks.find(buttonText);
    if (it != g_buttonCallbacks.end())
    {
        int ref = it->second;
        lua_State* L = g_luaState;
        if (L)
        {
            lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
            lua_pushstring(L, buttonText);
            if (lua_pcall(L, 1, 0, 0) != LUA_OK)
            {
                Log::Client::Write("[LUA] Callback error: %s", lua_tostring(L, -1));
                lua_pop(L, 1);
            }
            luaL_unref(L, LUA_REGISTRYINDEX, ref);
            g_buttonCallbacks.erase(it);
        }
    }

    free(const_cast<char*>(buttonText));
}

//Move to another class
int LuaEngine::Lua_IFaceMsgBox(lua_State* L)
{
    const char* title = luaL_checkstring(L, 1);
    const char* message = luaL_checkstring(L, 2);
    const char* buttonText = luaL_optstring(L, 3, nullptr);
    IFace::MsgBox(title, message, buttonText);
    return 0;
}

int LuaEngine::Lua_IFaceMsgBoxWithCallbacks(lua_State* L)
{
    const char* title = luaL_checkstring(L, 1);
    const char* message = luaL_checkstring(L, 2);
    luaL_checktype(L, 3, LUA_TTABLE);

    std::vector<MBEventCallback*> buttons;
    buttons.reserve(3);

    lua_pushnil(L);
    while (lua_next(L, 3) != 0)
    {
        const char* buttonText = lua_tostring(L, -2);
        if (!buttonText || !lua_isfunction(L, -1))
        {
            lua_pop(L, 1);
            continue;
        }

        lua_pushvalue(L, -1);
        int ref = luaL_ref(L, LUA_REGISTRYINDEX);
        g_buttonCallbacks[buttonText] = ref;

        char* textCopy = (char*)malloc(strlen(buttonText) + 1);
        if (textCopy)
        {
            strcpy_s(textCopy, strlen(buttonText) + 1, buttonText);
        }

        MBEventCallback* btn = MBEventCallback::Create(buttonText, buttonText, LuaButtonCallback, reinterpret_cast<unsigned int>(textCopy));
        if (btn)
        {
            buttons.push_back(btn);
        }

        lua_pop(L, 1);
    }

    if (buttons.size() >= 1 && buttons.size() <= 3)
    {
        IFace::MsgBox(title, message, buttons[0], buttons.size() >= 2 ? buttons[1] : nullptr, buttons.size() >= 3 ? buttons[2] : nullptr);
    }
    else
    {
        Log::Client::Write("[LUA] IFace.MsgBoxEx: too many buttons (max 3)");
    }

    return 0;
}

//tmp internal
int LuaEngine::Lua_TmpFn(lua_State* L)
{
    Terrain::RenderTerrainMap("NewMap.tga", 1024, 0x1, 0x1);
    return 0;
}

int LuaEngine::Lua_LogClientWrite(lua_State* L)
{
    const char* msg = luaL_checkstring(L, 1);
    Log::Client::Write("%s", msg);
    return 0;
}

int LuaEngine::Lua_Sleep(lua_State* L)
{
    int ms = luaL_checkinteger(L, 1);
    Sleep(ms);
    return 0;
}

/*int LuaEngine::Lua_Test(lua_State* L)
{
    TerrainData::SessionStart();

    for (U32 x = 0; x < Terrain::CellWidth() + 1; x++)
    {
        for (U32 y = 0; y < Terrain::CellHeight() + 1; y++)
        {
            float randomHeight = (float)rand() / RAND_MAX * 2.0f;
            TerrainData::SessionModifyHeight(x, y, randomHeight);
        }
    }

    TerrainData::SessionEnd();
    return 0;
}*/

int LuaEngine::Lua_Test(lua_State* L)
{
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    float height = (float)luaL_checknumber(L, 3);

    U32 width = Terrain::CellWidth() + 1;
    U32 heightCells = Terrain::CellHeight() + 1;
    if (x < 0 || x >= (int)width || y < 0 || y >= (int)heightCells)
    {
        return 0;
    }

    TerrainData::SessionStart();
    TerrainData::SessionModifyHeight(x, y, height);
    TerrainData::SessionEnd();

    return 0;
}

int LuaEngine::Lua_Test2(lua_State* L)
{
    U32 width = Terrain::CellWidth();
    lua_pushinteger(L, width);
    return 1;
}

int LuaEngine::Lua_Test3(lua_State* L)
{
    U32 height = Terrain::CellHeight();
    lua_pushinteger(L, height);
    return 1;
}


void LuaEngine::RegisterFunctions()
{
    auto setFuncs = [](lua_State* L, const std::vector<std::pair<const char*, lua_CFunction>>& funcs)
    {
        lua_newtable(L);
        for (const auto& p : funcs)
        {
            lua_pushcfunction(L, p.second);
            lua_setfield(L, -2, p.first);
        }
    };

    // Log table
    setFuncs(L, {});
    lua_newtable(L);
    setFuncs(L, { {"Write", Lua_LogClientWrite} });
    lua_setfield(L, -2, "Client");
    lua_setglobal(L, "Log");

    // Console
    setFuncs(L,
        {
            {"ProcessCmd", Lua_ConsoleProcessCmd},
            {"Message",    Lua_ConsoleMessage},
            {"SetType",    Lua_ConsoleSetType}
        });
    lua_setglobal(L, "Console");

    // VarSys
    setFuncs(L,
        {
            {"CreateCmd", Lua_VarSysCreateCmd}
        });
    lua_setglobal(L, "VarSys");

    // Client.Events
    lua_newtable(L);
    setFuncs(L,
        {
            {"CreateParticle", Lua_ClientEventsCreateParticle},
            {"CreateParticleCrc", Lua_ClientEventsCreateParticleCrc},
            {"SelectUnit", Lua_ClientEventsSelectUnit},
            {"UpdateSelectedLists", Lua_ClientEventsUpdateSelectedLists},
            {"TriggerResponse", Lua_ClientEventsTriggerResponse},
            {"TriggerClientMode", Lua_ClientEventsTriggerClientMode}
        });
    lua_setfield(L, -2, "Events");
    lua_setglobal(L, "Client");

    // Fun commands
    lua_newtable(L);
    setFuncs(L,
        {
            {"RandomColor", Lua_FunRandomColor}
        });
    lua_setglobal(L, "Fun");

    // Orders::Game::Move.Generate()
    lua_newtable(L);
    lua_newtable(L);
    lua_newtable(L);
    setFuncs(L,
        {
            {"Generate", Lua_OrdersGameMoveGenerate}
        });
    lua_setfield(L, -2, "Move");
    lua_setfield(L, -2, "Game");
    lua_setglobal(L, "Orders");

    // Internals
    lua_newtable(L);

    // Internals.State
    setFuncs(L,
        {
            {"ToggleGameUpdates", Lua_InternalsToggleGameUpdates},
            {"IsGameUpdating", Lua_InternalsIsGameUpdating},
            {"GetGameState", Lua_InternalsGetGameState}
        });
    lua_setfield(L, -2, "State");

    // Internals.Keybinds
    setFuncs(L,
        {
            {"Bind", Lua_InternalsBind},
            {"Unbind", Lua_InternalsUnbind}
        });
    lua_newtable(L);
    lua_pushcfunction(L, Lua_KeybindsGetProperty);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, Lua_KeybindsSetProperty);
    lua_setfield(L, -2, "__newindex");
    lua_setmetatable(L, -2);
    lua_setfield(L, -2, "Keybinds");
    lua_setglobal(L, "Internals");

    // Multiplayer.Data
    lua_newtable(L);
    lua_newtable(L);
    setFuncs(L,
        {
            {"Send", Lua_MultiplayerDataSend}
        });
    lua_setfield(L, -2, "Data");
    lua_setglobal(L, "Multiplayer");

    // SquadManager
    lua_newtable(L);
    setFuncs(L,
        {
            {"Create", Lua_SquadManagerCreate},
            {"Add", Lua_SquadManagerAdd},
            {"Select", Lua_SquadManagerSelect},
            {"JumpTo", Lua_SquadManagerJumpTo}
        });
    lua_setglobal(L, "SquadManager");

    // IFace
    lua_newtable(L);
    setFuncs(L,
        {
            {"MsgBox", Lua_IFaceMsgBox},
            {"MsgBoxEx", Lua_IFaceMsgBoxWithCallbacks}
        });
    lua_setglobal(L, "IFace");

    // Internal
    lua_pushcfunction(L, Lua_Sleep);
    lua_setglobal(L, "sleep");

    lua_pushcfunction(L, Lua_TmpFn);
    lua_setglobal(L, "testterrain");

    lua_pushcfunction(L, Lua_Test);
    lua_setglobal(L, "TerrainGen");

    lua_newtable(L);
    setFuncs(L,
        {
            {"CellWidth", Lua_Test2},
            {"CellHeight", Lua_Test3},
        });
    lua_setglobal(L, "Terrain");

    RegisterMapObjManager(L, manager);
}