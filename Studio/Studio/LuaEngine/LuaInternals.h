#pragma once
#include <LuaJIT/Src/lua.hpp>
#include <Windows.h>

int Lua_InternalsToggleGameUpdates(lua_State* L);
int Lua_InternalsIsGameUpdating(lua_State* L);
int Lua_InternalsGetGameState(lua_State* L);