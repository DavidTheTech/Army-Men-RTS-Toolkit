#include "Log.h"
#include <Windows.h>
#include <cstdio>

typedef void(__cdecl* LogClientWrite_t)(const char* format, ...);
static LogClientWrite_t LogClientWrite = (LogClientWrite_t)(Memory::ScanAddress(0x4CFD00));

void Log::Client::Write(const char* format, ...)
{
    char buffer[1024] = { 0 };

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    LogClientWrite("%s", buffer);
}