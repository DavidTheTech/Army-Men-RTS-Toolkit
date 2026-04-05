#include "LuaInternals.h"

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
        WaitForSingleObject(forcingThread, 1000);
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