#include "VarSys.h"

typedef int(__fastcall* VarSysCreateInteger_t)(char* IntName, int IntValue, int Unk2, DWORD* IntToAttach, int Unk3);
typedef void(__fastcall* VarSysCreateCmd_t)(char* IntName, int Unk1, int Unk2);
typedef int(__fastcall* VarSysCreateString_t)(char* a1, const char* a2, int a3, DWORD* a4, int a5);
typedef int(__fastcall* VarSysCreateFloat_t)(const char* path, float value, unsigned long flagsIn, DWORD* varPtr, void* context);
typedef int(__fastcall* VarSysRegisterHandler_t)(int IntName, int IntFunc, int Unk1);

static VarSysCreateInteger_t VarSysCreateInteger = (VarSysCreateInteger_t)(Memory::ScanAddress(0x4E4430));
static VarSysCreateCmd_t VarSysCreateCmd = (VarSysCreateCmd_t)(Memory::ScanAddress(0x4E4520));
static VarSysCreateString_t VarSysCreateString = (VarSysCreateString_t)(Memory::ScanAddress(0x4E43E0));
static VarSysCreateFloat_t VarSysCreateFloat = (VarSysCreateFloat_t)(Memory::ScanAddress(0x4E4480));
static VarSysRegisterHandler_t VarSysRegisterHandler = (VarSysRegisterHandler_t)(Memory::ScanAddress(0x4E4220));

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
