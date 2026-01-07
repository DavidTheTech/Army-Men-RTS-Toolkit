#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

namespace StyxNet
{
    class Client
    {
    public:
        static void LockSession();
        static void UnlockSession();
    };
}
