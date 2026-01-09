#include "Message.h"
#include <cstdio>

typedef void(__cdecl* Message_TriggerGameMessage_t)(U32 messageCrc, U32 params, ...);

static Message_TriggerGameMessage_t TriggerGameMessage_Fn = (Message_TriggerGameMessage_t)Memory::ScanAddress(0x54C220);

void Message::TriggerGameMessage(U32 messageCrc, U32 params, ...)
{
    va_list args;
    va_start(args, params);
    TriggerGameMessage_Fn(messageCrc, params, &args);
    va_end(args);
}
