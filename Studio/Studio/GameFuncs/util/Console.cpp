#include "Console.h"

typedef void(__fastcall* Console_ProcessCmd_t)(const char* cmd, int a2, int a3);

static Console_ProcessCmd_t ProcessCmd_Fn = (Console_ProcessCmd_t)(Memory::ScanAddress(0x004E1900));

void Console::ProcessCmd(const char* cmd, int a2, int a3)
{
    ProcessCmd_Fn(cmd, a2, a3);
}