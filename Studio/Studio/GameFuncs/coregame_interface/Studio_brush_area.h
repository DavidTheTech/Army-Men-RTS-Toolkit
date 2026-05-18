#pragma once
#include "..\..\Memory\Memory.h"

namespace Studio
{
    namespace Brush
    {
        class AreaBase
        {
        public:
            static void Constructor(void* thisPtr, const char* name);
            static void Notification(void* thisPtr, U32 crc, DWORD* e);
        };
    }
}