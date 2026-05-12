#include "TerrainData.h"

typedef void(__cdecl* TerrainData_SessionStart_t)();
typedef void(__cdecl* TerrainData_SessionEnd_t)();
typedef void(__fastcall* TerrainData_SessionModifyHeight_t)(int x, int y, float height);

static TerrainData_SessionStart_t SessionStart_Fn = (TerrainData_SessionStart_t)(Memory::ScanAddress(0x54FD30));
static TerrainData_SessionEnd_t SessionEnd_Fn = (TerrainData_SessionEnd_t)(Memory::ScanAddress(0x54FD70));
static TerrainData_SessionModifyHeight_t SessionModifyHeight_Fn = (TerrainData_SessionModifyHeight_t)(Memory::ScanAddress(0x54FF80));

void TerrainData::SessionStart()
{
    SessionStart_Fn();
}

void TerrainData::SessionEnd()
{
    SessionEnd_Fn();
}

void TerrainData::SessionModifyHeight(int x, int y, float height)
{
    SessionModifyHeight_Fn(x, y, height);
}