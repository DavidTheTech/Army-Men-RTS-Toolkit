#include "StyxNet_Client.h"

#define NETWORK_CLIENT_ADDR 0x0072961C

typedef int(__thiscall* LockSession_t)(DWORD* client);
typedef int(__thiscall* UnlockSession_t)(DWORD* client);

static LockSession_t StyxNetClient_LockSession_Fn = (LockSession_t)(Memory::ScanAddress(0x4F1830));
static UnlockSession_t StyxNetClient_UnlockSession_Fn = (UnlockSession_t)(Memory::ScanAddress(0x4F1850));

void StyxNet::Client::LockSession()
{
    DWORD* client = *(DWORD**)Memory::ScanAddress(NETWORK_CLIENT_ADDR);
    if (client)
    {
        StyxNetClient_LockSession_Fn(client);
    }
}

void StyxNet::Client::UnlockSession()
{
    DWORD* client = *(DWORD**)Memory::ScanAddress(NETWORK_CLIENT_ADDR);
    if (client)
    {
        StyxNetClient_UnlockSession_Fn(client);
    }
}