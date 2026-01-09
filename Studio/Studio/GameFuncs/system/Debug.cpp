#include "debug.h"

typedef void(__cdecl* debug_FPURegisters_t)(DWORD* Context);

static debug_FPURegisters_t FPURegisters_Fn = (debug_FPURegisters_t)(Memory::ScanAddress(0x4D0670));

void Debug::CallStack::FPURegisters(DWORD* Context)
{
    FPURegisters_Fn(Context);
}