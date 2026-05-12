#include "IFace.h"

typedef void(__fastcall* IFace_CmdHandler_t)(U32 pathCrc);
static IFace_CmdHandler_t CmdHandler_Fn = (IFace_CmdHandler_t)(Memory::ScanAddress(0x490030));

void IFace::CmdHandler(U32 pathCrc)
{
    CmdHandler_Fn(pathCrc);
}