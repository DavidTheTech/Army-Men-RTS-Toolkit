#include "Orders_game_move.h"
#include "../../Utils/Internals.h"

typedef void(__fastcall* Orders_Game_Move_Generate_t)(DWORD player, Internals::Math::Vector3& terrainPos, bool attack, int modifier, char a5);
static Orders_Game_Move_Generate_t Generate_Fn = (Orders_Game_Move_Generate_t)(Memory::ScanAddress(0x610F00));

void Orders::Game::Move::Generate(DWORD player, Internals::Math::Vector3 terrainPos, bool attack, int modifier, char a5)
{
	return Generate_Fn(player, terrainPos, attack, modifier, a5);
}