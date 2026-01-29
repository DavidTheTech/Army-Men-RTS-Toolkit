#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

namespace MultiPlayer
{
    class Data
    {
    public:
        static void Send(unsigned long crc, unsigned long size, const unsigned char* data, bool sync);
    };
}