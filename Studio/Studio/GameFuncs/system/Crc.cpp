#include "Crc.h"

typedef U32(__fastcall* Crc_CalcStr_t)(const char* str, U32 crc);

static Crc_CalcStr_t CalcStr_Fn = (Crc_CalcStr_t)(Memory::ScanAddress(0x4D9A30));

U32 Crc::CalcStr(const char* str, U32 crc)
{
    return CalcStr_Fn(str, crc);
}