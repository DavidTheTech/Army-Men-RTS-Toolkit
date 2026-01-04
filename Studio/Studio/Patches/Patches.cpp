#include "Patches.h"
#include "ConsoleEditPatch.h"

void Patches::ApplyAll()
{
    ConsoleEditPatch::Apply();
}
