#include "Vid.h"

typedef bool(__cdecl* Vid_InitDD_t)(bool noPick);
typedef bool(__cdecl* Vid_PickVidMode_t)();

static Vid_InitDD_t InitDD_Fn = (Vid_InitDD_t)(Memory::ScanAddress(0x43A1F0));
static Vid_PickVidMode_t PickVidMode_Fn = (Vid_PickVidMode_t)(Memory::ScanAddress(0x43B130));

bool Vid::InitDD(bool noPick)
{
    return InitDD_Fn(noPick);
}

bool Vid::PickVidMode()
{
    return PickVidMode_Fn();
}