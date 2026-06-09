#include "Terrain.h"
#include <stdio.h>
#include "../../Utils/Internals.h"

typedef int(__cdecl* TerrainSetShroud_t)(bool on);
typedef bool(__cdecl* Terrain_Report_t)(bool all);
typedef U32(__fastcall* Terrain_RenderTerrainMap_t)(const char* filename , U32 size, DWORD doColor, DWORD doOverlay);
typedef DWORD*(__fastcall* Terrain_GetCluster_t)(U32 x, U32 z);
typedef DWORD*(__fastcall* Terrain_GetCluster_float_t)(float x, float z);
typedef DWORD*(__fastcall* Terrain_GetCell_t)(S32 x, S32 z);

static TerrainSetShroud_t TerrainSetShroud = (TerrainSetShroud_t)(Memory::ScanAddress(0x434A80));
static Terrain_Report_t Report_Fn = (Terrain_Report_t)(Memory::ScanAddress(0x438DD0));
static Terrain_RenderTerrainMap_t RenderTerrainMap_Fn = (Terrain_RenderTerrainMap_t)(Memory::ScanAddress(0x651790));

static Terrain_GetCluster_t GetCluster_Fn = (Terrain_GetCluster_t)(Memory::ScanAddress(0x435910));
static Terrain_GetCluster_float_t GetCluster_Float_Fn = (Terrain_GetCluster_float_t)(Memory::ScanAddress(0x435890));
static Terrain_GetCell_t GetCell_Fn = (Terrain_GetCell_t)(Memory::ScanAddress(0x0));


int Terrain::SetShroud(bool on)
{
    return TerrainSetShroud(on);
}

bool Terrain::Report(bool all)
{
    return Report_Fn(all);
}

U32 Terrain::RenderTerrainMap(const char* filename, U32 size, DWORD doColor, DWORD doOverlay)
{
    //printf("%s %d %d %d\n", filename, size, doColor, doOverlay);
    return RenderTerrainMap_Fn(filename, size, doColor, doOverlay);
}

typedef F32(__thiscall* Terrain_FindFloor_t)(float* surfNormal, float, float);
static Terrain_FindFloor_t FindFloor_Fn = (Terrain_FindFloor_t)(Memory::ScanAddress(0x435640));

F32 Terrain::FindFloor(float* surfNormal, F32 z, F32 x)
{
    return FindFloor_Fn(surfNormal, z, x);
}

//Variables
U32 Terrain::CellHeight()
{
    return *reinterpret_cast<unsigned long*>(0x71EF18);
}

U32 Terrain::CellWidth()
{
    return *reinterpret_cast<unsigned long*>(0x71EF10);
}

U32 Terrain::ClusHeight()
{
    return *reinterpret_cast<unsigned long*>(0x71EF00);
}

U32 Terrain::ClusWidth()
{
    return *reinterpret_cast<unsigned long*>(0x71EF04);
}

DWORD* Terrain::GetCluster(U32 x, U32 z)
{
    return GetCluster_Fn(x, z);
}

DWORD* Terrain::GetCluster_float(float x, float z)
{
    return GetCluster_Float_Fn(x, z);
}

BYTE* Terrain::GetCell(S32 x, S32 z)
{
    DWORD cellMapX = *(DWORD*)0x76DDCC;
    DWORD* cellMapPtr = (DWORD*)0x76FA34;
    BYTE* cellMap = (BYTE*)(*cellMapPtr);

    DWORD index = (z * cellMapX + x) * 6;

    return &cellMap[index];
}