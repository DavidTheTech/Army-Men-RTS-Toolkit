#include <windows.h>

#include "Hooks\Hooks.h"
#include "Patches\Patches.h"

#include "GameFuncs\game\GameGod.h"
#include "GameFuncs\main\Runcodes.h"
#include "GameFuncs\coregame_interface\Studio.h"
#include "GameFuncs\system\Log.h"

HANDLE SetupEverythingHandle = (HANDLE)NULL;
HANDLE MinHookHandle = (HANDLE)NULL;

inline void WaitForTrue(volatile bool* flag, DWORD sleepMs = 1)
{
    while (!*flag)
    {
        Sleep(sleepMs);
    }
}

void SetupEverything()
{
    Patches::ApplyAll();

    WaitForTrue(GameGod::IsInitialized());

    DWORD runCodes = 0x007288E0;
    RunCodes::Register((DWORD*)runCodes, "Studio", (int)Studio::Process, (int)Studio::Init, (int)Studio::Done, (int)Studio::PostInit, 0);

}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            //IMPORTANT HOOKS ARE THE FIRST THING TO BE SETUP
            //LOG_DIAG("[STUDIO DLL]: MinHook (thread creation)", 1);
            Log::Client::Write("[STUDIO DLL]: MinHook");
            MinHookHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Hooks::MinHookStuff, NULL, 0, NULL);

            SetupEverythingHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SetupEverything, NULL, NULL, NULL);
        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

