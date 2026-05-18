#include "debug.h"

typedef void(__cdecl* debug_FPURegisters_t)(DWORD* Context);
static debug_FPURegisters_t FPURegisters_Fn = (debug_FPURegisters_t)(Memory::ScanAddress(0x4D0670));

typedef void* (__cdecl* DebugMemory_NewOperator_t)(size_t bytes);
static DebugMemory_NewOperator_t New_Operator_Fn = (DebugMemory_NewOperator_t)Memory::ScanAddress(0x4CFAB0);

void* Debug::Memory::NewOperator(size_t bytes)
{
    return New_Operator_Fn(bytes);
}

void Debug::CallStack::FPURegisters(DWORD* Context)
{
    FPURegisters_Fn(Context);
}