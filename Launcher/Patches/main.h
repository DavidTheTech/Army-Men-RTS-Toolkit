#pragma once
#include <windows.h>
#include <ctime>

static bool* g_initialized = reinterpret_cast<bool*>(0x6BAC8C);

static inline void WaitForTrue(volatile bool* flag, DWORD sleepMs = 1)
{
    while (!*flag)
    {
        Sleep(sleepMs);
    }
}

class StudioDLL
{
    
};