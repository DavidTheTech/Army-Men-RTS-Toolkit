#pragma once
#include <LuaJIT/Src/lua.hpp>
#include <Windows.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

//GameState
int Lua_InternalsToggleGameUpdates(lua_State* L);
int Lua_InternalsIsGameUpdating(lua_State* L);
int Lua_InternalsGetGameState(lua_State* L);

//Keybinds
int Lua_InternalsBind(lua_State* L);
int Lua_InternalsUnbind(lua_State* L);
int Lua_KeybindsGetProperty(lua_State* L);
int Lua_KeybindsSetProperty(lua_State* L);
void ProcessKeybindings(lua_State* L);