#include "Log.h"
#include <Windows.h>

typedef void(__cdecl* LogClientWrite_t)(const char* format, ...);
static LogClientWrite_t LogClientWrite = (LogClientWrite_t)(Memory::ScanAddress(0x4CFD00));

void Log::Client::Write(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    LogClientWrite(format, args);
    va_end(args);
}
