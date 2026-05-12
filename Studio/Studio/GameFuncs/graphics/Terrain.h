#pragma once
#include "..\..\Memory\Memory.h"

class Terrain
{
public:
    static int SetShroud(bool on);
    static bool Report(bool all);
    static U32 RenderTerrainMap(const char* filename, U32 size, DWORD doColor, DWORD doOverlay);
    static U32 CellWidth();
    static U32 CellHeight();
};