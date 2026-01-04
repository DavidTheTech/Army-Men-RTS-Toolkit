#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class RunCodes
{
public:
    static void Register(DWORD* runCodes, const char* a2, int a3, int a4, int a5, int a6, int a7);
    static int Set(DWORD* runCodes, unsigned int a1);
};
