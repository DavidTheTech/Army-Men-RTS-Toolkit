#include "IFace_Util.h"

typedef void(__fastcall* IFace_ScreenDump_t)(const char* mask);

static IFace_ScreenDump_t ScreenDump_Fn = (IFace_ScreenDump_t)(Memory::ScanAddress(0x48A420));

void IFace::ScreenDump(const char* mask)
{
    ScreenDump_Fn(mask);
}