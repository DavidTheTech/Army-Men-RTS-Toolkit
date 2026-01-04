#include "RunCodes.h"

typedef void(__thiscall* RegisterRuncode)(DWORD* thisptr, const char* a2, int a3, int a4, int a5, int a6, int a7);
typedef int(__thiscall* SetRuncode)(DWORD* runCodes, unsigned int a1);

static RegisterRuncode RegRunCode = (RegisterRuncode)(Memory::ScanAddress(0x4B6520));
static SetRuncode RunCodesSet = (SetRuncode)(Memory::ScanAddress(0x4B6750));

void RunCodes::Register(DWORD* runCodes, const char* a2, int a3, int a4, int a5, int a6, int a7)
{
    RegRunCode(runCodes, a2, a3, a4, a5, a6, a7);
}

int RunCodes::Set(DWORD* runCodes, unsigned int a1)
{
    return RunCodesSet(runCodes, a1);
}
