#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class GameTime
{
public:
    static void Pause(bool triggerMessage);
    static bool Paused();
    static void StepOnce();
    static void Readjust();
    static void Synchronize();
    static void SetFastMode(bool flag);
    static bool GetFastMode();
    static bool GetFastForwardMode();
    static void SetDisplayMode(bool flag);
    static bool GetDisplayMode();
    static float TimeSinceCycle(unsigned long cycle);
    static void IncGameCycle();
};
