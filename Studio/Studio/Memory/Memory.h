#pragma once
#include <Windows.h>
#include "..\GameFuncs\system\defines.h"

class Memory
{
public:
    static DWORD ScanAddress(DWORD address)
    {
        return address - 0x400000 + reinterpret_cast<DWORD>(GetModuleHandleA("AMRTS.exe"));
    }
};
