#include <windows.h>

#include "Hooks\Hooks.h"
#include "Patches\Patches.h"

#include "GameFuncs\system\defines.h"

#include "GameFuncs\game\GameGod.h"
#include "GameFuncs\main\Runcodes.h"
#include "GameFuncs\coregame_interface\Studio.h"
#include "GameFuncs\system\Log.h"
#include "GameFuncs\system\Utils.h"
#include "GameFuncs\graphics\Vid.h"
#include "GameFuncs\graphics\Terrain.h"
#include "GameFuncs\multiplayer\MultiPlayer.h"

#include "LuaEngine\LuaEngine.h"
#include "Settings.h"
#include "GameFuncs/system/Crc.h"


HANDLE SetupEverythingHandle = (HANDLE)NULL;
HANDLE MinHookHandle = (HANDLE)NULL;
HANDLE LuaEngineHandle = (HANDLE)NULL;

inline void WaitForTrue(volatile bool* flag, DWORD sleepMs = 1)
{
    while (!*flag)
    {
        Sleep(sleepMs);
    }
}

void SetupLuaEngine()
{
    WaitForTrue(GameGod::IsInitialized());

    LuaEngine lua;
    lua.Initialize();

    lua.SetVariable("testvar", 42);
    lua.LoadScript("main.lua");
}

void SetupEverything()
{
    Settings settings;
    settings.LoadJson();

    Patches::ApplyAll();

    WaitForTrue(GameGod::IsInitialized());

    //Studio
    DWORD runCodes = 0x7288E0;
    RunCodes::Register((DWORD*)runCodes, "Studio", (int)Studio::Process, (int)Studio::Init, (int)Studio::Done, (int)Studio::PostInit, 0);

    if (settings.AutoLaunch)
    {
        Sleep(2000);
        RunCodes::Set((DWORD*)runCodes, "Studio");
    }
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            //IMPORTANT HOOKS ARE THE FIRST THING TO BE SETUP
            Log::Client::Write("[STUDIO DLL]: Hooks::Setup");
            MinHookHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Hooks::Setup, NULL, 0, NULL);

            Log::Client::Write("[STUDIO DLL]: SetupEverything");
            SetupEverythingHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SetupEverything, NULL, NULL, NULL);

            Log::Client::Write("[STUDIO DLL]: Lua Engine");
            LuaEngineHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SetupLuaEngine, NULL, NULL, NULL);

        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

