#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class TerrainData
{
public:
    static void SessionStart();
    static void SessionEnd();
    static void SessionModifyHeight(int x, float height, int z);

    static void BlendEntireMap();

    static void SessionAddArea(DWORD* rectX, int* rectZ);
};