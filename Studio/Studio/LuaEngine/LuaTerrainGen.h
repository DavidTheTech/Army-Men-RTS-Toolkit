#pragma once
#include <LuaJIT/Src/lua.hpp>
#include <time.h>
#include <corecrt_math.h>
#include <string>

#include "../GameFuncs/system/defines.h"

#include "../GameFuncs/graphics/Terrain.h"
#include "../GameFuncs/coregame/TerrainData.h"

#include "../GameFuncs/system/Debug.h"
#include "../GameFuncs/coregame/Coregame.h"
#include "../GameFuncs/coregame/Game.h"

typedef struct
{
    F32	x;
    F32	y;
    F32	z;
    F32	w;
} QUADRUPLET;

typedef struct
{
    QUADRUPLET right;
    QUADRUPLET up;
    QUADRUPLET front;
    QUADRUPLET posit;
} MATRIX_STRUCT;


struct Matrix
{
    union
    {
        struct
        {
            Internals::Math::Vector3 right;
            F32 rightw;

            Internals::Math::Vector3 up;
            F32 upw;

            Internals::Math::Vector3 front;
            F32 frontw;

            Internals::Math::Vector3 posit;
            F32 positw;
        };
        MATRIX_STRUCT Mat;
    };
};

int Lua_TerrainTest(lua_State* L);

int Lua_TerrainSetRGB(lua_State* L);
int Lua_TerrainGen(lua_State* L);

int Lua_TerrainAddWaterC(lua_State* L);
int Lua_TerrainAddWaterF(lua_State* L);

int Lua_TerrainDeleteObjs(lua_State* L);

int Lua_TerrainBlend(lua_State* L);
int Lua_TerrainPaint(lua_State* L);

int Lua_TerrainActiveTypeCount(lua_State* L);
int Lua_TerrainPlace(lua_State* L);

int Lua_TerrainFindFloor(lua_State* L);


int Lua_TerrainCellWidth(lua_State* L);
int Lua_TerrainCellHeight(lua_State* L);
int Lua_TerrainClusWidth(lua_State* L);
int Lua_TerrainClusHeight(lua_State* L);




//tmp

int Lua_TerrainSave(lua_State* L);