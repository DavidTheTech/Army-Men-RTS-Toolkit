#include "Patches.h"
#include <stdio.h>
#include "ConsoleEditPatch.h"
#include "SingleInstancePatch.h"
#include "../GameFuncs/system/Log.h"

void Patches::ApplyAll()
{
    //Re-adds the ingame console
    //would crash if you called \/ ConsoleEdit
    //CreateControl("Edit", "ConsoleEdit")
    Log::Client::Write("ConsoleEditPatch::Apply()");
    ConsoleEditPatch::Apply();


    //MOVED TO Hooks.cpp
    //Allows multiple instances of the game to run at once
    //instead of using a sandbox like sandboxie

    //SingleInstancePatch::Apply();
}
