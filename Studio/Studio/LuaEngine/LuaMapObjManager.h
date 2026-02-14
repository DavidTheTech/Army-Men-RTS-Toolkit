#pragma once
#include <LuaJIT/Src/lua.hpp>
#include "..\MapObjManager.h"

void RegisterMapObjManager(lua_State* L, MapObjManager& manager);