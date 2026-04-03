#pragma once

#include "../GameFuncs/util/VarSys.h"
#include "../GameFuncs/game/GameGod.h"
#include "../GameFuncs/game/Missions.h"
#include "../Utils/Internals.h"

class Game
{
public:
	void Setup();
	void Loop();
};

extern Game g_game;