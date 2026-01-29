#include "Vid.h"

typedef U32(__cdecl* Vid_Config_SelectCard_t)();
static Vid_Config_SelectCard_t SelectCard_Fn = (Vid_Config_SelectCard_t)(Memory::ScanAddress(0x453BC0));

U32 Vid::Config::SelectCard()
{
	return SelectCard_Fn();
}