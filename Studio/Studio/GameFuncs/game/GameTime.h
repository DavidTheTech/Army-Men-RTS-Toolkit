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
    static void SetFastMode();
};
