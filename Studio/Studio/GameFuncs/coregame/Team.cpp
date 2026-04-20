#include "Team.h"

typedef DWORD* (__thiscall* Team_SetColor_t)(DWORD* team, const Color &c);
static Team_SetColor_t SetColor_Fn = (Team_SetColor_t)Memory::ScanAddress(0x549310);

typedef DWORD* (__fastcall* Team_GetRelation_t)(DWORD* outResult, DWORD* team1, DWORD* team2);
static Team_GetRelation_t GetRelation_Fn = (Team_GetRelation_t)Memory::ScanAddress(0x5490F0);

DWORD* Team::SetColor(DWORD* team, const Color& c)
{
	return SetColor_Fn(team, c);
}

DWORD Team::GetRelation(DWORD* team1, DWORD* team2)
{
    DWORD result;
    GetRelation_Fn(&result, team1, team2);
    return result;
}