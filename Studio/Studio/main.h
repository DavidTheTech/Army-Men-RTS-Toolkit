#pragma once
#include <windows.h>
#include <ctime>

#include "Settings.h"
#include "Hooks\Hooks.h"
#include "Patches\Patches.h"
#include "LuaEngine\LuaEngine.h"
#include "Utils\CursorLocking.h"
#include "Utils\GameConsole.h"
#include "Utils\Internals.h"
#include "Handlers\Handlers.h"
#include "Server\Server.h"

#include "GameFuncs\system\defines.h"
#include "GameFuncs\game\GameGod.h"
#include "GameFuncs\main\Runcodes.h"
#include "GameFuncs\coregame_interface\Studio.h"
#include "GameFuncs\system\Log.h"
#include "GameFuncs\system\Utils.h"
#include "GameFuncs\graphics\Vid.h"
#include "GameFuncs\graphics\Terrain.h"
#include "GameFuncs\multiplayer\MultiPlayer_Data.h"
#include "GameFuncs\multiplayer\MultiPlayer_Host.h"
#include "GameFuncs\system\Crc.h"
#include "GameFuncs\styxnet\Styxnet_Client.h"
#include "GameFuncs\util\Console.h"
#include "GameFuncs\multiplayer\MultiPlayer_Host.h"
#include "GameFuncs\system\Debug.h"
#include "GameFuncs\interface\IFace_Util.h"
#include "GameFuncs\game\Missions.h"
#include "MapObjManager.h"
#include "Testing\Testing.h"
#include "Game\Game.h"

class StudioDLL
{
public:
    static const char* credits;

};