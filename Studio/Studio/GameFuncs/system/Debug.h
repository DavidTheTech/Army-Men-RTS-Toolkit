#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

namespace Debug
{
    namespace CallStack
    {
        void FPURegisters(DWORD* Context);
    }
};