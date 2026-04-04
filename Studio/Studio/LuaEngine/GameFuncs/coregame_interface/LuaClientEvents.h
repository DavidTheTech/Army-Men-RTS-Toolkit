#pragma once
#include "../../LuaEngine.h"
#include "../../../GameFuncs/coregame_interface/Client_events.h"

int Lua_ClientEventsCreateParticle(lua_State* L);
int Lua_ClientEventsCreateParticleCrc(lua_State* L);
int Lua_ClientEventsSelectUnit(lua_State* L);
int Lua_ClientEventsUpdateSelectedLists(lua_State* L);
int Lua_ClientEventsTriggerResponse(lua_State* L);
int Lua_ClientEventsTriggerClientMode(lua_State* L);