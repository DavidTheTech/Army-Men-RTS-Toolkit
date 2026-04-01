#pragma once
#include <Windows.h>
#include <string>

#include "../../Utils/Internals.h"
#include "../../Memory/Memory.h"

class Missions
{
public:
    static const char* GetName();
    static const char* GetNameDate();
    static DWORD GetSelected();
};

