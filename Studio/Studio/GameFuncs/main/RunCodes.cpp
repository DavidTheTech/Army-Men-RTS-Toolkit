#include "RunCodes.h"

typedef void(__thiscall* RegisterRuncode_t)(DWORD* thisptr, const char* a2, int a3, int a4, int a5, int a6, int a7);
typedef int(__thiscall* SetRuncod_t)(DWORD* runCodes, unsigned int a1);
typedef int(__thiscall* SetRuncode2_t)(DWORD* runCodes, const char *s);

static RegisterRuncode_t RegRunCode = (RegisterRuncode_t)(Memory::ScanAddress(0x4B6520));
static SetRuncod_t RunCodesSet = (SetRuncod_t)(Memory::ScanAddress(0x4B6750));
static SetRuncode2_t RunCodesSet2 = (SetRuncode2_t)(Memory::ScanAddress(0x4B6700));

void RunCodes::Register(DWORD* runCodes, const char* a2, int a3, int a4, int a5, int a6, int a7)
{
    RegRunCode(runCodes, a2, a3, a4, a5, a6, a7);
}

int RunCodes::Set(DWORD* runCodes, unsigned int a1)
{
    return RunCodesSet(runCodes, a1);
}

int RunCodes::Set(DWORD* runCodes, const char *s)
{
    return RunCodesSet2(runCodes, s);
}