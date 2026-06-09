#include "Utils.h"

typedef unsigned long(__cdecl* Utils_Strlen_t)(const char* str);
typedef char* (__fastcall* Utils_Strmcpy_t)(char* Destination, char* Source, size_t Count);

static Utils_Strlen_t Strlen_Fn = (Utils_Strlen_t)(Memory::ScanAddress(0x4D57D0));
static Utils_Strmcpy_t Strmcpy_Fn = (Utils_Strmcpy_t)(Memory::ScanAddress(0x4D58D0));

unsigned long Utils::Strlen(const char* str)
{
    return Strlen_Fn(str);
}

char* Utils::Strmcpy(char* Destination, char* Source, size_t Count)
{
    return Strmcpy_Fn(Destination, Source, Count);
}