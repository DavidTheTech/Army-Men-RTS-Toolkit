#pragma once

#include "../GameFuncs/util/VarSys.h"
#include "../GameFuncs/game/GameGod.h"
#include "../GameFuncs/game/Missions.h"
#include "../Utils/Internals.h"

class Game
{
public:
	static void SetupVars();
	void Setup();
	void Loop();

	static bool varsSetupDone;
};

extern Game g_game;

extern VarSys::VarIntegerFake terrainGenHeightMin;
extern VarSys::VarIntegerFake terrainGenHeightMax;

extern void* g_terrainMinVarItem;
extern void* g_terrainMaxVarItem;
