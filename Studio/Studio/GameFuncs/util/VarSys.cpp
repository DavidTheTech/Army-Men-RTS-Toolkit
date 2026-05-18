#include "VarSys.h"

//
typedef void(__fastcall* VarSysCreateCmd_t)(char* IntName, U32 flagsIn, void* context);

typedef int(__fastcall* VarSysCreateString_t)(const char* path, const char* value, U32 flagsIn, void* varPtr, void* context);
typedef int(__fastcall* VarSysCreateInteger_t)(const char* path, int value, U32 flagsIn, void* varPtr, void* context);
typedef int(__fastcall* VarSysCreateFloat_t)(const char* path, float value, U32 flagsIn, void* varPtr, void* context);

typedef int(__fastcall* VarSysRegisterHandler_t)(const char* path, DWORD* func, U32 flagsIn);
typedef void(__fastcall* VarSysRegisterHandlerFn)(const char* path, VarSysCallBack func, U32 flags);

typedef void(__thiscall* VarSysSetString_t)(void* varPtr, const char* strValue);

//
static VarSysCreateCmd_t VarSysCreateCmd = (VarSysCreateCmd_t)(Memory::ScanAddress(0x4E4520));

static VarSysCreateString_t VarSysCreateString = (VarSysCreateString_t)(Memory::ScanAddress(0x4E43E0));
static VarSysCreateInteger_t VarSysCreateInteger = (VarSysCreateInteger_t)(Memory::ScanAddress(0x4E4430));
static VarSysCreateFloat_t VarSysCreateFloat = (VarSysCreateFloat_t)(Memory::ScanAddress(0x4E4480));

static VarSysRegisterHandlerFn VarSys_RegisterHandler = (VarSysRegisterHandlerFn)(Memory::ScanAddress(0x4E4220));
static VarSysSetString_t SetString_Fn = (VarSysSetString_t)(Memory::ScanAddress(0x4E0120));


typedef void* (__cdecl* FindVarItem_t)(const char* path, void* context, bool required);
static FindVarItem_t VarSysFindVarItem = (FindVarItem_t)(Memory::ScanAddress(0x4E3D20));

typedef float(__thiscall* VarItemFloat_t)(void* varItem);
static VarItemFloat_t VarItemFloat = (VarItemFloat_t)(Memory::ScanAddress(0x4E30B0));

int VarSys::CreateString(const char* name, const char* value, U32 flagsIn, void* varPtr, void* context)
{
    return VarSysCreateString(name, value, flagsIn, varPtr, context);
}

int VarSys::CreateInteger(const char* name, int value, U32 flagsIn, void* varPtr, void* context)
{
    return VarSysCreateInteger(name, value, flagsIn, varPtr, context);
}

int VarSys::CreateFloat(const char* path, float value, U32 flagsIn, void* varPtr, void* context)
{
    //U32 floatBits = *(U32*)&value;
    //return (void*)VarSysCreateFloat(path, (int)flagsIn, floatBits, varPtr, context);
    return VarSysCreateFloat(path, value, flagsIn, varPtr, context);
}

void VarSys::CreateCmd(const char* name, U32 flagsIn, void* context)
{
    VarSysCreateCmd((char*)name, flagsIn, context);
}

void VarSys::RegisterHandler(const char* path, VarSysCallBack func, U32 flags)
{
    VarSys_RegisterHandler(path, func, flags);
}

void VarSys::SetString(void* varPtr, const char* strValue)
{
    return SetString_Fn(varPtr, strValue);
}

void* VarSys::FindVarItem(const char* path)
{
    return VarSysFindVarItem(path, nullptr, false);
}

float VarSys::GetFloat(void* varItem)
{
    return VarItemFloat(varItem);
}