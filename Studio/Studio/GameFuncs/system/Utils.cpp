#include "Utils.h"

typedef unsigned long(__cdecl* Utils_Strlen_t)(const char* str);

static Utils_Strlen_t Strlen_Fn = (Utils_Strlen_t)(Memory::ScanAddress(0x4D57D0));

unsigned long Utils::Strlen(const char* str)
{
    return Strlen_Fn(str);
}
