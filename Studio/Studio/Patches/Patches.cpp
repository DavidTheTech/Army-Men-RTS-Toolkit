#include "Patches.h"
#include <stdio.h>
#include "ConsoleEditPatch.h"
#include "SingleInstancePatch.h"

void Patches::ApplyAll()
{
    //Re-adds the ingame console
    //would crash if you called \/ ConsoleEdit
    //CreateControl("Edit", "ConsoleEdit")
    ConsoleEditPatch::Apply();

    //Allows multiple instances of the game to run at once
    //instead of using a sandbox like sandboxie
    SingleInstancePatch::Apply();
    printf("swag");
}
