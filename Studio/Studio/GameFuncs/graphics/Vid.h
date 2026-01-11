#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class Vid
{
public:
    static bool ToggleWindowedMode();//Look at hooks.cpp
    static bool SetMode(unsigned long mode, unsigned long width, unsigned long height, bool force);
    static bool InitSurfaces();
    static bool SetCoopLevel();
    static void OnActivate();
    static void RestoreSurfaces();
    static void Done();
    static bool OnMove();
    static bool OnSize(UINT nType, signed long cx, signed long cy);
    static bool NextDriver(signed long inc);
    static unsigned long FreeVidMem(unsigned long report);
    static DWORD FlagsToBitDepth(DWORD flags);
    static bool SetGamma(signed long val);
    static void LogPerf(const char* format, ...);
    static void ReportMode(unsigned long report);
    static unsigned long Report();

    //vid_enumdx
    static bool InitDD(bool noPick = false);
};