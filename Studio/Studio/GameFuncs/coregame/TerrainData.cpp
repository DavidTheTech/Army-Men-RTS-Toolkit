#include "TerrainData.h"

typedef void(__cdecl* TerrainData_SessionStart_t)();
typedef void(__cdecl* TerrainData_SessionEnd_t)();
typedef void(__fastcall* TerrainData_SessionModifyHeight_t)(int x, int y, float height);

typedef void(__cdecl* TerrainData_BlendEntireMap_t)();
typedef void(__fastcall* TerrainData_SessionAddArea_t)(DWORD* rectX, int* rectZ);

static TerrainData_SessionStart_t SessionStart_Fn = (TerrainData_SessionStart_t)(Memory::ScanAddress(0x54FD30));
static TerrainData_SessionEnd_t SessionEnd_Fn = (TerrainData_SessionEnd_t)(Memory::ScanAddress(0x54FD70));
static TerrainData_SessionModifyHeight_t SessionModifyHeight_Fn = (TerrainData_SessionModifyHeight_t)(Memory::ScanAddress(0x54FF80));
static TerrainData_SessionAddArea_t SessionAddArea_Fn = (TerrainData_SessionAddArea_t)(Memory::ScanAddress(0x5501C0));

static TerrainData_BlendEntireMap_t BlendEntireMap_Fn = (TerrainData_BlendEntireMap_t)(Memory::ScanAddress(0x54FB10));

void TerrainData::SessionStart()
{
    SessionStart_Fn();
}

void TerrainData::SessionEnd()
{
    SessionEnd_Fn();
}

void TerrainData::SessionModifyHeight(int x, float height, int z)
{
    SessionModifyHeight_Fn(x, z, height);
}

void TerrainData::BlendEntireMap()
{
    BlendEntireMap_Fn();
}

void TerrainData::SessionAddArea(DWORD* rectX, int* rectZ)
{
    SessionAddArea_Fn(rectX, rectZ);
}