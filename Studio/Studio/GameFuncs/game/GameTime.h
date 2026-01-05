#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class GameTime
{
public:
    static void Save(DWORD* scope);
    static void Load(DWORD* scope);
    static void DisplaySample(unsigned long sample);
    static float GameCycleOffset();
    static bool IsFallingBehind();
    static bool IsDroppingFrames();
    static bool IsStalled();
    static bool Test();
    static void Compute();
    static bool Cycle(bool force);
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
