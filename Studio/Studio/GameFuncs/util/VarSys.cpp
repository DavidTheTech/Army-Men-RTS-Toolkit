#include "VarSys.h"

typedef int(__fastcall* sub_004E4430)(char* IntName, int IntValue, int Unk2, DWORD* IntToAttach, int Unk3);
static sub_004E4430 VarSysCreateInteger = (sub_004E4430)(Memory::ScanAddress(0x4E4430));

typedef void(__fastcall* sub_4E4520)(char* IntName, int Unk1, int Unk2);
static sub_4E4520 VarSysCreateCmd = (sub_4E4520)(Memory::ScanAddress(0x4E4520));

typedef int(__fastcall* sub_004E43E0)(char* a1, const char* a2, int a3, DWORD* a4, int a5);
static sub_004E43E0 VarSysCreateString = (sub_004E43E0)(Memory::ScanAddress(0x4E43E0));

typedef int(__fastcall* sub_004E4480)(const char* path, float value, unsigned long flagsIn, DWORD* varPtr, void* context);
static sub_004E4480 VarSysCreateFloat = (sub_004E4480)(Memory::ScanAddress(0x4E4480));

typedef int(__fastcall* sub_004E4220)(int IntName, int IntFunc, int Unk1);
static sub_004E4220 VarSysRegisterHandler = (sub_004E4220)(Memory::ScanAddress(0x4E4220));

int VarSys::CreateInteger(const char* name, int value, int flagsIn, DWORD* varPtr, int context)
{
    return VarSysCreateInteger((char*)name, value, flagsIn, varPtr, context);
}

int VarSys::CreateString(const char* name, const char* value, int flagsIn, DWORD* varPtr, int context)
{
    return VarSysCreateString((char*)name, value, flagsIn, varPtr, context);
}

int VarSys::CreateFloat(const char* path, float value, unsigned long flags, DWORD* varPtr, void* context)
{
    return VarSysCreateFloat(path, value, flags, varPtr, context);
}

void VarSys::CreateCmd(const char* name, int unk1, int unk2)
{
    VarSysCreateCmd((char*)name, unk1, unk2);
}

int VarSys::RegisterHandler(int name, int func, int unk1)
{
    return VarSysRegisterHandler(name, func, unk1);
}
