#pragma once
#include <LuaJIT/Src/lua.hpp>
#include "../Utils/PerlinNoise.h"

int Lua_PerlinNoise(lua_State* L);
int Lua_PerlinNoiseScaled(lua_State* L);
int Lua_SetPerlinSeed(lua_State* L);