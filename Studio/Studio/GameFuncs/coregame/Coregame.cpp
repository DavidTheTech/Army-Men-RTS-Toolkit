#include "Coregame.h"

typedef void(__cdecl* CoreGame_SetTextSave_t)();
static CoreGame_SetTextSave_t SetTextSave_Fn = (CoreGame_SetTextSave_t)(Memory::ScanAddress(0x53F600));

void Coregame::SetTextSave()
{
    SetTextSave_Fn();
}
