#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class Console
{
public:
    static void ProcessCmd(const char* cmd, int a2, int a3);
};