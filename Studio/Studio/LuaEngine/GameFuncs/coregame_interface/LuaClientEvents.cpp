#include "LuaClientEvents.h"
#include "../../../GameFuncs/system/Crc.h"
#include "../../../MapObject.h"
#include "../../../Utils/Internals.h"

int Lua_ClientEventsCreateParticle(lua_State* L)
{
	DWORD* mouse = *(DWORD**)0x773194;
	float x = (float)luaL_checknumber(L, 1);
	float y = (float)luaL_checknumber(L, 2);
	float z = (float)luaL_checknumber(L, 3);
	Internals::Math::Vector3 location = { x, y, z };

	const char* crcStr = luaL_checkstring(L, 4);
	U32 tmpCrc = Crc::CalcStr(crcStr);
	int result = Client::Events::CreateParticle(mouse, location, tmpCrc);
	lua_pushinteger(L, result);

	return 1;
}

int Lua_ClientEventsCreateParticleCrc(lua_State* L)
{
	DWORD* mouse = *(DWORD**)0x773194;
	float x = (float)luaL_checknumber(L, 1);
	float y = (float)luaL_checknumber(L, 2);
	float z = (float)luaL_checknumber(L, 3);
	Internals::Math::Vector3 location = { x, y, z };

	const char* crcStr = luaL_checkstring(L, 4);
	U32 typeCrc = (U32)std::stoul(crcStr, nullptr, 16);
	int result = Client::Events::CreateParticle(mouse, location, typeCrc);
	lua_pushinteger(L, result);

	return 1;
}

int Lua_ClientEventsSelectUnit(lua_State* L)
{
	DWORD* mouse = *(DWORD**)0x773194;

	MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
	DWORD unitAddy = obj->objectAddress;

	int modifier = (int)luaL_optinteger(L, 2, 0);
	int rMod = 8;
	switch (modifier)
	{
		case 1: rMod = 9; break;
		case 2: rMod = 10; break;
		case 3: rMod |= 4; break;
		default: rMod = 8; break;
	}

	Client::Events::SelectUnit(unitAddy, rMod);
	return 0;
}

int Lua_ClientEventsUpdateSelectedLists(lua_State* L)
{
	DWORD mouse = *(DWORD*)0x773194;
	Client::Events::UpdateSelectedLists(mouse);
	return 0;
}

int Lua_ClientEventsTriggerResponse(lua_State* L)
{
	DWORD* mouse = *(DWORD**)0x773194;
	DWORD* unitObjList = (DWORD*)(*(DWORD*)0x773194 + 0xA8);

	const char* crcStr = luaL_checkstring(L, 1);
	U32 crc = (U32)std::stoul(crcStr, nullptr, 16);

	Client::Events::TriggerResponse(mouse, unitObjList, crc);
	return 0;
}

int Lua_ClientEventsTriggerClientMode(lua_State* L)
{
	DWORD* mouse = *(DWORD**)0x773194;
	int mode = (int)luaL_checkinteger(L, 1);

	Client::Events::TriggerClientMode(mouse, mode);
	return 0;
}