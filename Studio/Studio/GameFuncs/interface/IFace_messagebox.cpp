#include "IFace_messagebox.h"
#include "messagebox_event.h"

typedef void* (__cdecl* AllocMemOPNew_t)(size_t bytes);
static AllocMemOPNew_t AllocMemOPNew = (AllocMemOPNew_t)Memory::ScanAddress(0x4CFAB0);

typedef void* (__thiscall* MBEventCtor_t)(void* pThis, const char* ident, const char* caption);
static MBEventCtor_t MBEventCtor = (MBEventCtor_t)Memory::ScanAddress(0x4A6770);

typedef void* (__fastcall* MsgBox_t)(const char* title, const char* message, U32 flags, void* btn1, void* btn2, void* btn3);
static MsgBox_t RealMsgBox = (MsgBox_t)Memory::ScanAddress(0x4A6210);

static void* CreateMBEvent(const char* ident, const char* caption)
{
    void* event = AllocMemOPNew(0x64);
    if (event)
    {
        MBEventCtor(event, ident, caption);
    }
    return event;
}

void IFace::MsgBox(const char* title, const char* message, const char* buttonText)
{
    void* btn = buttonText ? CreateMBEvent(buttonText, buttonText) : nullptr;
    RealMsgBox(title, message, 0, btn, nullptr, nullptr);
}

void IFace::MsgBox(const char* title, const char* message, MBEvent* btn1, MBEvent* btn2, MBEvent* btn3)
{
    RealMsgBox(title, message, 0, btn1, btn2, btn3);
}