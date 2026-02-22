#pragma once
#include <windows.h>
#include <ctime>

class Internals
{
public:
    static const char* GetDate(bool u = 0);

    static inline void WaitForTrue(volatile bool* flag, DWORD sleepMs = 1)
    {
        while (!*flag)
        {
            Sleep(sleepMs);
        }
    }
};