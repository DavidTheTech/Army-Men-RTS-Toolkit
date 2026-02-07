#include "Orders.h"

typedef bool(__thiscall* Orders_LoadDemo_t)(DWORD* bFile);

static Orders_LoadDemo_t LoadDemo_Fn = (Orders_LoadDemo_t)(Memory::ScanAddress(0x6103C0));

bool Orders::LoadDemo(DWORD* bFile)
{
    return LoadDemo_Fn(bFile);
}