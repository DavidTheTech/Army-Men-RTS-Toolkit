#pragma once
#include "../../LuaEngine.h"
#include "../../../GameFuncs/util/Console.h"

//Console
int Lua_ConsoleProcessCmd(lua_State* L);
int Lua_ConsoleMessage(lua_State* L);
int Lua_ConsoleSetType(lua_State* L);