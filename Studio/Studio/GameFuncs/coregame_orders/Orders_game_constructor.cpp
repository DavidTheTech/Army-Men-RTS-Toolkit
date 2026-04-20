#include "Orders_game_constructor.h"

typedef void(__fastcall* Orders_Game_Constructor_Generator_t)(DWORD player, U32 id, U32 operation, U32 value);
static Orders_Game_Constructor_Generator_t Generate_Fn = (Orders_Game_Constructor_Generator_t)(Memory::ScanAddress(0x613D50));

void Orders::Game::Constructor::Generate(DWORD player, U32 id, U32 operation, U32 value)
{
    return Generate_Fn(player, id, operation, value);
}