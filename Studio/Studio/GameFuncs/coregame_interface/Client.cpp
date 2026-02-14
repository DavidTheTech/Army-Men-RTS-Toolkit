#include "Client.h"

typedef DWORD(__cdecl* Client_GetMouse_t)();
typedef DWORD(__stdcall* Client_GetPlayer_t)();

static Client_GetMouse_t GetMouse_Fn = (Client_GetMouse_t)(Memory::ScanAddress(0x5BC020));
static Client_GetPlayer_t GetPlayer_Fn = (Client_GetPlayer_t)(Memory::ScanAddress(0x5C0730));

DWORD Client::GetMouse()
{
    return GetMouse_Fn();
}

DWORD Client::GetPlayer()
{
    return GetPlayer_Fn();
}