#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"
#include "defines.h"

class Crc
{
public:
    static U32 CalcStr(const char* str, U32 crc = 0xFFFFFFFF);
};
