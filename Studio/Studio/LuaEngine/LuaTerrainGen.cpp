#include "LuaTerrainGen.h"

//MOVE TO THEIR OWN CLASSES
typedef DWORD(__fastcall* GameObjCtrl_FindTypeCrc_t)(U32 crc);
static GameObjCtrl_FindTypeCrc_t FindTypeCrc_Fn = (GameObjCtrl_FindTypeCrc_t)(Memory::ScanAddress(0x53F920));

typedef DWORD* (__thiscall* GameObjCtrl_FindTypeCrc2_t)(DWORD* typeList, U32 crc, DWORD* rootNode);
static GameObjCtrl_FindTypeCrc2_t FindTypeCrc2_Fn = (GameObjCtrl_FindTypeCrc2_t)(Memory::ScanAddress(0x539EC0));

typedef void(__thiscall* SpawnClosest_t)(DWORD type, Internals::Math::Vector3* pos, DWORD team, int a4, float a5);
static SpawnClosest_t SpawnClosest_Fn = (SpawnClosest_t)(0x5F24A0);

//DWORD* FASTCALL ObjectNewOnMap(_DWORD *type, DWORD *pos, int id, int zip, int complete)
typedef DWORD(__fastcall* MapObjCtrl_ObjectNewOnMap_t)(DWORD* mapObjType, float* matrix, int id, int zip, int complete);
static MapObjCtrl_ObjectNewOnMap_t ObjectNewOnMap_Fn = (MapObjCtrl_ObjectNewOnMap_t)(0x5FD5C0);

typedef BOOL(__stdcall* sub_5F61C0_t)(int a1);
static sub_5F61C0_t sub_5F61C0_Fn = (sub_5F61C0_t)(0x5F61C0);
//END

DWORD* GetMapObjType(const char* typeName)
{
    U32 crc = Crc::CalcStr(typeName, 0);
    DWORD* typeList = (DWORD*)0x773660;
    DWORD* rootNode = *(DWORD**)0x773664;

    DWORD* result = FindTypeCrc2_Fn(typeList, crc, rootNode);
    if (result && *result == crc && result[1] && sub_5F61C0_Fn(0x4542B0F8))
    {
        return (DWORD*)result[1];
    }

    return nullptr;
}

int Lua_TerrainSetRGB(lua_State* L)
{
    int x = luaL_checkinteger(L, 1);
    int z = luaL_checkinteger(L, 2);
    int r = luaL_checkinteger(L, 3);
    int g = luaL_checkinteger(L, 4);
    int b = luaL_checkinteger(L, 5);

    DWORD cellListPtr = *(DWORD*)0x71EF34;
    DWORD cellPitch = *(DWORD*)0x71EF14;

    DWORD data = cellListPtr + 0x10 * (x + z * cellPitch);
    DWORD cell = *(DWORD*)data;
    DWORD cellColor = *(DWORD*)(data + 0x08);
    //printf("X:%d Z:%d 0x%08X = 0x%08X (Color: 0x%08X)\n", z, x, data, cell, cellColor);

    U32 color = (0xFF << 24) | (r << 16) | (g << 8) | b;

    *(DWORD*)(data + 0x08) = color;
    return 1;
}

int Lua_TerrainCellWidth(lua_State* L)
{
    U32 width = Terrain::CellWidth();
    lua_pushinteger(L, width);
    return 1;
}

int Lua_TerrainCellHeight(lua_State* L)
{
    U32 height = Terrain::CellHeight();
    lua_pushinteger(L, height);
    return 1;
}

int Lua_TerrainClusWidth(lua_State* L)
{
    U32 width = Terrain::ClusWidth();
    lua_pushinteger(L, width);
    return 1;
}

int Lua_TerrainClusHeight(lua_State* L)
{
    U32 height = Terrain::ClusHeight();
    lua_pushinteger(L, height);
    return 1;
}

int Lua_TerrainGen(lua_State* L)
{
    int x = luaL_checkinteger(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    int z = luaL_checkinteger(L, 3);

    U32 width = Terrain::CellWidth() + 1;
    U32 heightCells = Terrain::CellHeight() + 1;

    if (x < 0 || x >= (int)width || z < 0 || z >= (int)heightCells)
    {
        return 0;
    }

    TerrainData::SessionStart();
    TerrainData::SessionModifyHeight(x, y, z);
    TerrainData::SessionEnd();

    return 0;
}

int Lua_TerrainFindFloor(lua_State* L)
{
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float z = (float)luaL_checknumber(L, 3);

    F32 height = Terrain::FindFloor(NULL, x, z);

    lua_pushnumber(L, (lua_Number)height);
    return 1;
}

void ClusterToCells(int clusterX, int clusterZ, DWORD& left, DWORD& right, int& top, int& bottom)
{
    left = clusterX * 4;
    right = left + 3;
    top = clusterZ * 4;
    bottom = top + 3;
}

int Lua_TerrainAddWaterC(lua_State* L)
{
    int x = luaL_checkinteger(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    int z = luaL_checkinteger(L, 3);
    bool enable = lua_toboolean(L, 4) != 0;

    TerrainData::SessionStart();

    DWORD* cluster = Terrain::GetCluster(x, z);

    if (cluster)
    {
        DWORD* clusterData = *(DWORD**)cluster;
        if (clusterData)
        {
            DWORD clusterAddr = (DWORD)cluster;
            float* waterHeight = (float*)(clusterAddr + 0x40);
            DWORD* waterStatus = (DWORD*)(clusterAddr + 0x3C);

            //printf("Cluster [%d,%d] - Ptr: 0x%p, Data: 0x%p, Height: %f, Status: %d\n", x, z, cluster, clusterData, *waterHeight, *waterStatus & 1);

            if (enable)
            {
                *waterHeight = y;
                *waterStatus = 0x21;
            }
            else
            {
                *waterStatus = 0x20;
            }

            DWORD left, right;
            int top, bottom;
            ClusterToCells(x, z, left, right, top, bottom);

            DWORD rectX[2] = { left, right };
            int rectZ[2] = { top, bottom };
            TerrainData::SessionAddArea(rectX, rectZ);
        }
    }

    TerrainData::SessionEnd();
    return 0;
}

int Lua_TerrainAddWaterF(lua_State* L)
{
    int x = luaL_checkinteger(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    int z = luaL_checkinteger(L, 3);
    bool enable = lua_toboolean(L, 4) != 0;

    TerrainData::SessionStart();

    DWORD* cluster = Terrain::GetCluster_float(x, z);

    if (cluster)
    {
        DWORD* clusterData = *(DWORD**)cluster;
        if (clusterData)
        {
            DWORD clusterAddr = (DWORD)cluster;
            //printf("0x%08X\n", clusterAddr);
            float* waterHeight = (float*)(clusterAddr + 0x40);
            DWORD* waterStatus = (DWORD*)(clusterAddr + 0x3C);

            //printf("Cluster [%d,%d] - Ptr: 0x%p, Data: 0x%p, Height: %f, Status: %d\n", x, z, cluster, clusterData, *waterHeight, *waterStatus & 1);

            if (enable)
            {
                *waterHeight = y;
                *waterStatus = 0x21;
            }
            else
            {
                *waterStatus = 0x20;
            }

            //DWORD left, right;
            //int top, bottom;
            //ClusterToCells(x, z, left, right, top, bottom);

            //DWORD rectX[2] = { left, right };
            //int rectZ[2] = { top, bottom };
            //SessionAddArea_Fn(rectX, rectZ);
        }
    }

    TerrainData::SessionEnd();
    return 0;
}

int Lua_TerrainDeleteObjs(lua_State* L)
{
    typedef int(__fastcall* sub_005FDDC0)();
    sub_005FDDC0 MapObjCtrlGetOnMapList = (sub_005FDDC0)(Memory::ScanAddress(0x5FDDC0));

    typedef int(__thiscall* sub_5EF840)(DWORD obj);
    sub_5EF840 MarkForDeletion = (sub_5EF840)(Memory::ScanAddress(0x5EF840));

    DWORD* listHead = *(DWORD**)(MapObjCtrlGetOnMapList() + 4);
    while (listHead && *listHead)
    {
        //objects.emplace_back(*listHead);
        DWORD realObj = *listHead;
        MarkForDeletion(realObj);

        listHead = reinterpret_cast<DWORD*>(listHead[2]);
    }
    return 0;
}

void PaintTerrainCell(int x, int z, U8 terrainType)
{
    BYTE* cell = Terrain::GetCell(x, z);

    if (cell)
    {
        BYTE oldTerrain = cell[0];
        cell[0] = terrainType;

        //printf("Cell at %p: x=%d, z=%d, old terrain=%d, new terrain=%d\n", cell, x, z, oldTerrain, terrainType);
    }
}

int Lua_TerrainActiveTypeCount(lua_State* L)
{
    U32 typesCount = *(U32*)0x770660;
    lua_pushinteger(L, typesCount);
    return 1;
}

int Lua_TerrainBlend(lua_State* L)
{
    TerrainData::BlendEntireMap();
    return 0;
}

int Lua_TerrainPaint(lua_State* L)
{
    int x = luaL_checknumber(L, 1);
    int z = luaL_checknumber(L, 2);

    //U8 terrainType = ((U8)(rand() % 2) + 1) - 1;
    int terrainType = luaL_checknumber(L, 3);

    PaintTerrainCell(x, z, terrainType);
    return 0;
}

int Lua_TerrainSave(lua_State* L)
{
    int ver = luaL_checknumber(L, 1);
    const char* name = luaL_checkstring(L, 2);
    std::string mapName = std::string(name) + std::to_string(ver);

    //printf("MapName_num %s, %d\n", mapName.c_str(), ver);

    Coregame::SetTextSave();
    if (Game::SaveMission("missions\\terraingen", mapName.c_str()))
    {
        //printf("saved to mp/%s\n", mapName.c_str());
    }

    return 0;
}


int Lua_TerrainPlace(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);
    float x = (float)luaL_checknumber(L, 2);
    float y = (float)luaL_checknumber(L, 3);
    float z = (float)luaL_checknumber(L, 4);

    DWORD team = *(DWORD*)(0x76F8B8);
    U32 crc = Crc::CalcStr(name, 0);
    DWORD type = FindTypeCrc_Fn(crc);

    DWORD* mapObjType = GetMapObjType(name);

    float yaw = 0;
    float pitch = 0;
    float roll = 0;

    if (lua_gettop(L) >= 5)
    {
        roll = (float)luaL_checknumber(L, 5);
    }

    if (lua_gettop(L) >= 6)
    {
        pitch = (float)luaL_checknumber(L, 6);
    }

    if (lua_gettop(L) >= 7)
    {
        yaw = (float)luaL_checknumber(L, 7);
    }

    //printf("Yaw: %.04f, Pitch: %.04f, Roll: %.04f\n", yaw, pitch, roll);

    float yawRad = yaw * (3.14159265359f / 180.0f);
    float pitchRad = pitch * (3.14159265359f / 180.0f);
    float rollRad = roll * (3.14159265359f / 180.0f);

    float cy = cos(yawRad);
    float sy = sin(yawRad);
    float cp = cos(pitchRad);
    float sp = sin(pitchRad);
    float cr = cos(rollRad);
    float sr = sin(rollRad);

    /*float matrix[16] =
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    };*/

    //wrong math/diff objects have different yaw/pitch/roll order
    float matrix[16] =
    {
        cy * cr + sy * sp * sr, sy * cp, cy * sr - sy * sp * cr, 0,
        -sy * cr + cy * sp * sr, cy * cp, -sy * sr - cy * sp * cr, 0,
        cp * sr, -sp, cp * cr, 0,
        x, y, z, 1
    };

    if (mapObjType)
    {
        ObjectNewOnMap_Fn(mapObjType, matrix, 0, 0, FALSE);
    }

    return 0;
}

int Lua_TerrainTest(lua_State* L)
{
    printf("==== LUA TERRAIN TEST ====\n");
    printf("you shouldn't be calling this from lua\n");
    printf("=*=*=*=*=*=*=*=\n");
    return 0;
}

//old test stuff
/*
int Lua_TerrainTest(lua_State* L)
{
    printf("====START====\n");

    U32 w = Terrain::CellWidth();
    U32 h = Terrain::CellHeight();

    printf("x%d : z%d\n", w, h);

    DWORD cellListPtr = *(DWORD*)0x71EF34;
    DWORD cellPitch = *(DWORD*)0x71EF14;

    /*
    for (U32 z = 0; z <= h; ++z)
    {
        for (U32 x = 0; x <= w; ++x)
        {
            DWORD data = cellListPtr + 0x10 * (z + x * cellPitch);
            DWORD cell = *(DWORD*)data;
            DWORD cellColor = *(DWORD*)(data + 0x08);
            printf("X:%d Z:%d 0x%08X = 0x%08X (Color: 0x%08X)\n", z, x, data, cell, cellColor);

            U32 r = rand() % 256;
            U32 g = rand() % 256;
            U32 b = rand() % 256;
            U32 color = (0xFF << 24) | (r << 16) | (g << 8) | b;

            *(DWORD*)(data + 0x08) = color;
        }
    }

FILE* f = nullptr;
if (fopen_s(&f, "test.bmp", "rb") != 0 || !f)
{
    printf("failed\n");
    return 0;
}

fseek(f, 18, SEEK_SET);
U32 width, height;
fread(&width, 4, 1, f);
fread(&height, 4, 1, f);

fseek(f, 54, SEEK_SET);

for (U32 z = 0; z <= h; ++z)
{
    for (U32 x = 0; x <= w; ++x)
    {
        // Map terrain coordinates to image coordinates (wrap or clamp)
        U32 imgX = x % width;
        U32 imgY = z % height;

        // Calculate position in BMP file for this pixel
        U32 pixelOffset = (imgY * width + imgX) * 3;
        fseek(f, 54 + pixelOffset, SEEK_SET);

        U8 b, g, r;
        fread(&b, 1, 1, f);
        fread(&g, 1, 1, f);
        fread(&r, 1, 1, f);

        DWORD data = cellListPtr + 0x10 * (z + x * cellPitch);

        U32 color = (0xFF << 24) | (r << 16) | (g << 8) | b;
        *(DWORD*)(data + 0x08) = color;
    }
}

fclose(f);

printf("====END====\n");
//randTerrainColors();
return 1;
}*/