#include "Client.h"

//Client::
typedef DWORD(__cdecl* Client_GetMouse_t)();
typedef DWORD(__stdcall* Client_GetPlayer_t)();

//Client::Cmd
typedef void(__thiscall* Client_Cmd_LogTasks_t)(DWORD* obj);

//Client::
static Client_GetMouse_t GetMouse_Fn = (Client_GetMouse_t)(Memory::ScanAddress(0x5BC020));
static Client_GetPlayer_t GetPlayer_Fn = (Client_GetPlayer_t)(Memory::ScanAddress(0x5C0730));

//Client::Cmd
static Client_Cmd_LogTasks_t LogTasks_Fn = (Client_Cmd_LogTasks_t)(Memory::ScanAddress(0x5C34E0));

DWORD Client::GetMouse()
{
    return GetMouse_Fn();
}

DWORD Client::GetPlayer()
{
    return GetPlayer_Fn();
}

void Client::Cmd::LogTasks(DWORD* obj)
{
    return LogTasks_Fn(reinterpret_cast<DWORD*>(obj));
}