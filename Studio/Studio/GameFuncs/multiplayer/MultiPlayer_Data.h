#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

namespace MultiPlayer
{
    namespace Data
    {
        void Send(unsigned long crc, unsigned long size, const unsigned char* data, bool sync);
    }
}