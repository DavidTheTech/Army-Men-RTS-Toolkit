#include "LuaTeam.h"
#include "../GameFuncs/util/Console.h"
#include <string>
#include "../GameFuncs/system/Debug.h"
#include "../GameFuncs/system/Utils.h"
#include "../GameFuncs/system/Crc.h"

struct GameIdent
{
	char str[65];  // 64 + null
	unsigned int crc;
};

typedef void(__thiscall* Team_Constructor_t)(void* thisPtr, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, int a20);
static Team_Constructor_t Team_Constructor_Fn = (Team_Constructor_t)(0x5440E0);

typedef void(__thiscall* GameIdent_Constructor_t)(void* thisPtr, const char* source);
static GameIdent_Constructor_t GameIdent_Constructor_Fn = (GameIdent_Constructor_t)(0x5EBA40);

typedef unsigned int(__cdecl* Team_NewId_t)();
static Team_NewId_t Team_NewId_Fn = (Team_NewId_t)(0x548BB0);

U32 NewId()
{
	return Team_NewId_Fn();
}

int Lua_CreateTeam(lua_State* L)
{
	//CreateRegion
	const char* RegionName = luaL_checkstring(L, 1);
	int startX = luaL_checkinteger(L, 2);
	int startZ = luaL_checkinteger(L, 3);
	int endX = luaL_checkinteger(L, 4);
	int endZ = luaL_checkinteger(L, 5);
	const char* teamName = luaL_checkstring(L, 6);

	//50/50 either move to GameFuncs or keep this, depends on the outcome
	std::string FullCmdRegion = "coregame.region.create " + std::string(RegionName) + " " + std::to_string(startX) + " " + std::to_string(startZ) + " " + std::to_string(endX) + " " + std::to_string(endZ);
	//printf(FullCmdRegion.c_str());
	Console::ProcessCmd(FullCmdRegion.c_str(), 0, 0);
	//^WORKING PERFECTLY^

	char nameStruct[68];
	GameIdent_Constructor_Fn(nameStruct, teamName);

	unsigned int teamId = NewId();
	//printf("teamId %d\n", teamId);
	void* team = Debug::Memory::NewOperator(0x418);

	Team_Constructor_Fn(
		team,
		*(int*)&nameStruct[0],
		*(int*)&nameStruct[4],
		*(int*)&nameStruct[8],
		*(int*)&nameStruct[12],
		*(int*)&nameStruct[16],
		*(int*)&nameStruct[20],
		*(int*)&nameStruct[24],
		*(int*)&nameStruct[28],
		*(int*)&nameStruct[32],
		*(int*)&nameStruct[36],
		*(int*)&nameStruct[40],
		*(int*)&nameStruct[44],
		*(int*)&nameStruct[48],
		*(int*)&nameStruct[52],
		*(int*)&nameStruct[56],
		*(int*)&nameStruct[60],
		*(int*)&nameStruct[64],
		0,
		teamId
	);

	delete team;

	
	return 0;
}