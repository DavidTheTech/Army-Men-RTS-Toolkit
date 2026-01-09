#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class Message
{
public:
    static void TriggerGameMessage(U32 messageCrc, U32 params, ...);
};