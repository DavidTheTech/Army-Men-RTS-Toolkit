#include "Client_events.h"
#include "../../Utils/Internals.h"


typedef bool(__thiscall* Client_Events_CreateParticle_t)(DWORD* mouse, Internals::Math::Vector3 &location, U32 typeCrc);
static Client_Events_CreateParticle_t CreateParticle_Fn = (Client_Events_CreateParticle_t)(Memory::ScanAddress(0x5C0040));

typedef void(__thiscall* Client_Events_SelectUnit_t)(DWORD* mouse, DWORD unit, int flags);
static Client_Events_SelectUnit_t SelectUnit_Fn = (Client_Events_SelectUnit_t)(Memory::ScanAddress(0x5C3610));

typedef void(__thiscall* Client_Events_UpdateSelectedLists_t)(DWORD mouse);
static Client_Events_UpdateSelectedLists_t UpdateSelectedLists_Fn = (Client_Events_UpdateSelectedLists_t)(Memory::ScanAddress(0x5BFEA0));

typedef void(__thiscall* Client_Events_TriggerResponse_t)(DWORD* mouse, DWORD* unitObjList, U32 crc);
static Client_Events_TriggerResponse_t TriggerResponse_Fn = (Client_Events_TriggerResponse_t)(Memory::ScanAddress(0x5BF930));

typedef void(__thiscall* Client_Events_TriggerClientMode_t)(DWORD* mouse, int mode);
static Client_Events_TriggerClientMode_t TriggerClientMode_Fn = (Client_Events_TriggerClientMode_t)(Memory::ScanAddress(0x5C0760));

bool Client::Events::CreateParticle(DWORD* mouse, Internals::Math::Vector3& location, U32 typeCrc)
{
	return CreateParticle_Fn(mouse, location, typeCrc);
}

void Client::Events::SelectUnit(DWORD unit, int flags)
{
	DWORD* mouse = *(DWORD**)0x773194;
	return SelectUnit_Fn(mouse, unit, flags);
}

void Client::Events::UpdateSelectedLists(DWORD mouse)
{
	return UpdateSelectedLists_Fn(mouse);
}

void Client::Events::TriggerResponse(DWORD* mouse, DWORD* unitObjList, U32 crc)
{
	TriggerResponse_Fn(mouse, unitObjList, crc);
}

void Client::Events::TriggerClientMode(DWORD* mouse, int mode)
{
	TriggerClientMode_Fn(mouse, mode);
}