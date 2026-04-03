#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

typedef void(__fastcall* VarSysCallBack)(U32);

class VarSys
{
public:
    
    enum
    {
        DEFAULT = 0x0000,
        NOTIFY = 0x0001,
        NOEDIT = 0x0004,
        CLAMP = 0x0008,
        RAWVAR = 0x0010,
        RAWDATA = 0x0020,
    };

    struct VarStringFake
    {
        void* vtable;
        DWORD itemReaper[3];//CHANGE IF MORE STUFF NEEDS TO BE DONE FOR VarString
    };

    struct VarIntegerFake
    {
        void* vtable;
        DWORD itemReaper[3];
    };

    struct VarFloatFake
    {
        void* vtable;
        DWORD itemReaper[3];
    };

    static int CreateString(const char* path, const char* value, U32 flagsIn = DEFAULT, void* varPtr = NULL, void* context = NULL);
    static int CreateInteger(const char* path, int value, U32 flagsIn = DEFAULT, void* varPtr = NULL, void* context = NULL);
    static int CreateFloat(const char* path, float value, U32 flagsIn = DEFAULT, void* varPtr = NULL, void* context = NULL);

    static void CreateCmd(const char* name, U32 flagsIn = DEFAULT, void* context = NULL);
    static void RegisterHandler(const char* path, VarSysCallBack func, U32 flags = 0x0000);

    static void SetString(void* varPtr, const char* strValue);
};
