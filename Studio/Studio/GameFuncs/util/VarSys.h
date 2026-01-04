#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class VarSys
{
public:
    static int CreateInteger(const char* name, int value, int flagsIn, DWORD* varPtr, int context);
    static int CreateString(const char* name, const char* value, int flagsIn, DWORD* varPtr, int context);
    static int CreateFloat(const char* path, float value, unsigned long flags, DWORD* varPtr, void* context);
    static void CreateCmd(const char* name, int unk1, int unk2);
    static int RegisterHandler(int name, int func, int unk1);
};
