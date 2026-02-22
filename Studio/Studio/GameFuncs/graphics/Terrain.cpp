#include "Terrain.h"
#include <stdio.h>

typedef int(__cdecl* TerrainSetShroud_t)(bool on);
typedef bool(__cdecl* Terrain_Report_t)(bool all);
typedef U32(__fastcall* Terrain_RenderTerrainMap_t)(const char* filename , U32 size, DWORD doColor, DWORD doOverlay);

static TerrainSetShroud_t TerrainSetShroud = (TerrainSetShroud_t)(Memory::ScanAddress(0x434A80));
static Terrain_Report_t Report_Fn = (Terrain_Report_t)(Memory::ScanAddress(0x438DD0));
static Terrain_RenderTerrainMap_t RenderTerrainMap_Fn = (Terrain_RenderTerrainMap_t)(Memory::ScanAddress(0x651790));

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