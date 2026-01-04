#include "GameGod.h"

static bool* g_initialized = reinterpret_cast<bool*>(0x6BAC8C);

bool *GameGod::IsInitialized()
{
    return g_initialized;
}
