#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

typedef void (*VarSysCallBack)(U32);

class VarSys
{
public:
    static int CreateInteger(const char* name, int value, int flagsIn, DWORD* varPtr, int context);
    static int CreateString(const char* name, const char* value, int flagsIn, DWORD* varPtr, int context);
    static int CreateFloat(const char* path, float value, unsigned long flags, DWORD* varPtr, void* context);
    static void CreateCmd(const char* name, int unk1 = 0, int unk2 = 0);
    static void RegisterHandler(const char* path, VarSysCallBack func, U32 flags = 0x0000);
};
