#pragma once
#include "..\..\Memory\Memory.h"

namespace Log
{
    class Client
    {
    public:
        static void Write(const char* format, ...);
    };
}
