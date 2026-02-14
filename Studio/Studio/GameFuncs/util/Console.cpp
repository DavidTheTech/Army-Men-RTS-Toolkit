#include "Console.h"
#include <cstdio>

typedef void(__fastcall* Console_ProcessCmd_t)(const char* cmd, int a2, int a3);
typedef void(__cdecl* Console_Message_t)(const char* format, ...);
typedef void(__thiscall* Console_SetType_t)(U32 type);

static Console_ProcessCmd_t ProcessCmd_Fn = (Console_ProcessCmd_t)(Memory::ScanAddress(0x4E1900));
static Console_Message_t Message_Fn = (Console_Message_t)(Memory::ScanAddress(0x4E19A0));
static Console_SetType_t SetType_Fn = (Console_SetType_t)(Memory::ScanAddress(0x4E1990));

void Console::ProcessCmd(const char* cmd, int a2, int a3)
{
    ProcessCmd_Fn(cmd, a2, a3);
}

void Console::Message(const char* format, ...)
{
    char buffer[1024] = { 0 };

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Message_Fn("%s", buffer);
}

//TODO need to fix as still prints to console even after changin type
//even when the value 0x7560C8(Console::currentType) is set
//????
void Console::SetType(U32 type)
{
    //printf("0x%08X\n", type);
    SetType_Fn(type);
}