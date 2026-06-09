#pragma once
#include "..\..\Memory\Memory.h"
#include "..\..\Utils\Internals.h"
#include "../../GameFuncs/system/Crc.h"

class Terrain
{
public:
    static int SetShroud(bool on);
    static bool Report(bool all);
    static U32 RenderTerrainMap(const char* filename, U32 size, DWORD doColor, DWORD doOverlay);
    
    static F32 FindFloor(float* surfNormal, F32 z, F32 x);
    
    static DWORD* GetCluster(U32 x, U32 z);
    static DWORD* GetCluster_float(float x, float y);
    static BYTE* GetCell(S32 x, S32 z);

    //variables
    static U32 CellWidth();
    static U32 CellHeight();
    static U32 ClusWidth();
    static U32 ClusHeight();
};