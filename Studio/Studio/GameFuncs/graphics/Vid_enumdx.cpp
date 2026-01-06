#include "Vid.h"

typedef bool(__cdecl* Vid_InitDD_t)(bool noPick);

static Vid_InitDD_t InitDD_Fn = (Vid_InitDD_t)(Memory::ScanAddress(0x43A1F0));

bool Vid::InitDD(bool noPick)
{
    return InitDD_Fn(noPick);
}