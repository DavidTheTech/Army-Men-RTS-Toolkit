#include "MultiPlayer.h"

typedef void(__fastcall* Multiplayer_Data_Send_t)(unsigned long crc, unsigned long size, const unsigned char* data, bool sync);

static Multiplayer_Data_Send_t Multiplayer_Data_Send = (Multiplayer_Data_Send_t)(Memory::ScanAddress(0x4BA300));

void MultiPlayer::Data::Send(unsigned long crc, unsigned long size, const unsigned char* data, bool sync)
{
    Multiplayer_Data_Send(crc, size, data, sync);
}
