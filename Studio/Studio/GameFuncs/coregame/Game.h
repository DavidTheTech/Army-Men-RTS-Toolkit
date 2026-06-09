#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class Game
{
public:
    static bool SaveMission(const char* path, const char* mission);
};