#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class TerrainData
{
public:
    static void SessionStart();
    static void SessionEnd();
    static void SessionModifyHeight(int x, int y, float height);
};