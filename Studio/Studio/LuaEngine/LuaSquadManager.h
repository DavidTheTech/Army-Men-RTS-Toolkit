#pragma once
#include <LuaJIT/Src/lua.hpp>
#include <Windows.h>
#include <string>

#include "../GameFuncs/util/Console.h"

int Lua_SquadManagerCreate(lua_State* L);
int Lua_SquadManagerAdd(lua_State* L);
int Lua_SquadManagerSelect(lua_State* L);
int Lua_SquadManagerJumpTo(lua_State* L);