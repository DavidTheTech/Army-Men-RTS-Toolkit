#include "Terrain.h"

typedef int(__cdecl* TerrainSetShroud_t)(bool on);

static TerrainSetShroud_t TerrainSetShroud = (TerrainSetShroud_t)(Memory::ScanAddress(0x434A80));

int Terrain::SetShroud(bool on)
{
    return TerrainSetShroud(on);
}