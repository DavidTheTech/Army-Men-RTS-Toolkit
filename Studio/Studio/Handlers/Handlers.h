#pragma once
#include <stdio.h>
#include "../GameFuncs/game/GameGod.h"
#include "../Utils/Internals.h"
#include "../GameFuncs/system/Log.h"

class Handlers
{
private:
    static void __fastcall CmdClientHandler(U32 value);

public:
    static void LoadHandlers();
};