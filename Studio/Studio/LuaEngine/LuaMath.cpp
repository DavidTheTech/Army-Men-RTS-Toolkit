#include "LuaMath.h"

static PerlinNoise g_PerlinNoise;

int Lua_PerlinNoise(lua_State* L)
{
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float result = g_PerlinNoise.Noise(x, y);
    lua_pushnumber(L, result);
    return 1;
}

int Lua_PerlinNoiseScaled(lua_State* L)
{
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float scale = (float)luaL_checknumber(L, 3);
    float result = g_PerlinNoise.NoiseScaled(x, y, scale);
    lua_pushnumber(L, result);
    return 1;
}

int Lua_SetPerlinSeed(lua_State* L)
{
    unsigned int seed = (unsigned int)luaL_checknumber(L, 1);
    g_PerlinNoise.SetSeed(seed);
    return 0;
}