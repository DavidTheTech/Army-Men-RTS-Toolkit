#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class Console
{
public:
    static void ProcessCmd(const char* cmd, int a2, int a3);
    static void Message(const char* format, ...);
    static void SetType(U32 type);
};