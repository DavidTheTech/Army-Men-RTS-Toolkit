#include "IFace.h"

typedef void(__fastcall* IFace_CmdHandler_t)(U32 pathCrc);
static IFace_CmdHandler_t CmdHandler_Fn = (IFace_CmdHandler_t)(Memory::ScanAddress(0x490030));

typedef DWORD* (__fastcall* IFace_FindByName_t)(const char*, DWORD*);
static IFace_FindByName_t FindByName_Fn = (IFace_FindByName_t)(Memory::ScanAddress(0x48DD40));

void IFace::CmdHandler(U32 pathCrc)
{
    CmdHandler_Fn(pathCrc);
}

DWORD* IFace::FindByName(const char* name, DWORD* base)
{
    return FindByName_Fn(name, base);
}