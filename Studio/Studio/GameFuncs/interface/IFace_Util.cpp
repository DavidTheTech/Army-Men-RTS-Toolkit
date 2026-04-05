#include "IFace_Util.h"

typedef void(__fastcall* IFace_CmdHandler_t)(const char* mask);
static IFace_CmdHandler_t CmdHandler_Fn = (IFace_CmdHandler_t)(Memory::ScanAddress(0x48A420));

namespace IFace
{
    void ScreenDump(const char* mask)
    {
        CmdHandler_Fn(mask);
    }
}