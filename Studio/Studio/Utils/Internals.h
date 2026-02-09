#pragma once
#include <windows.h>

class Internals
{
public:
    static inline void WaitForTrue(volatile bool* flag, DWORD sleepMs = 1)
    {
        while (!*flag)
        {
            Sleep(sleepMs);
        }
    }
};