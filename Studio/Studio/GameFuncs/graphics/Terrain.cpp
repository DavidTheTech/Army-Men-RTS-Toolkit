#include "Terrain.h"

typedef int(__cdecl* TerrainSetShroud_t)(bool on);
typedef bool(__cdecl* Terrain_Report_t)(bool all);

static TerrainSetShroud_t TerrainSetShroud = (TerrainSetShroud_t)(Memory::ScanAddress(0x434A80));
static Terrain_Report_t Report_Fn = (Terrain_Report_t)(Memory::ScanAddress(0x438DD0));

int Terrain::SetShroud(bool on)
{
    return TerrainSetShroud(on);
}

bool Terrain::Report(bool all)
{
    return Report_Fn(all);
}
