#include "main.h"
#include "Hooks.h"
#include "memory.h"
#include "Settings.h"
#include "CursorLocking.h"
#include <thread>

//extra shii for studio
typedef int(__thiscall* SetRuncode2_t)(DWORD* runCodes, const char* s);
static SetRuncode2_t RunCodesSet2 = (SetRuncode2_t)(Memory::ScanAddress(0x4B6700));

HANDLE SetupEverythingHandle = NULL;
HANDLE MinHookHandle = NULL;
HANDLE LuaEngineHandle = NULL;
HANDLE MultiplayerServerHandle = NULL;
HANDLE CursorLockHandle = NULL;
HANDLE HandlersHandle = NULL;
HANDLE TestingHandle = NULL;

void LockCursor(int cursorLockTimer)
{
    EnumWindows(EnumWindowsProc, 0);
    if (targetWindow == NULL)
    {
        return;
    }

    while (true)
    {
        GetWindowDimensions(targetWindow);
        if (IsWindowOnTop(targetWindow))
        {
            LockCursorInWindow();
        }
        Sleep(cursorLockTimer);
    }
}

void everything()
{
    WaitForTrue(g_initialized);

    //Sleep(5000);
    Sleep(3000);
    if (g_settings.BasicStudio)
    {
        Hooks::LaunchStudio();
    }

    if (g_settings.DoWeLockCursor)
    {
        CursorLockHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LockCursor, (LPVOID)(size_t)g_settings.CursorLockTimer, NULL, NULL);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);

        g_settings.LoadJson();
        Hooks::Setup();

        //Log::Client::Write("[STUDIO DLL]: Patches::ApplyAll");
        //Patches::ApplyAll();
        
        /*if (g_settings.DoWeLockCursor)
        {
            CursorLockHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LockCursor, (LPVOID)(size_t)g_settings.CursorLockTimer, NULL, NULL);
        }*/
        
        SetupEverythingHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)everything, NULL, NULL, NULL);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}