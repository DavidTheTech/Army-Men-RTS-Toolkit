#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

namespace IFace
{
    void CmdHandler(U32 pathCrc);

    DWORD* FindByName(const char* name, DWORD* base);
}