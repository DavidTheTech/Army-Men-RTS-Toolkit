#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class IFace
{
public:
    static void ScreenDump(const char* mask = "AMRTS_SD_%d.tga");
};