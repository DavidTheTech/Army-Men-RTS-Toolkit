#pragma once
#include <stdio.h>
#include "../GameFuncs/game/GameGod.h"
#include "../Utils/Internals.h"
#include "../GameFuncs/system/Log.h"

class Handlers
{
private:
    static void __fastcall CmdClientHandler(U32 value);
    static void __fastcall ClientDevCmdClientHandler(U32 value);

public:
    static void __fastcall StudioTerrainGenHandler(U32 value);
    static void LoadHandlers();
};