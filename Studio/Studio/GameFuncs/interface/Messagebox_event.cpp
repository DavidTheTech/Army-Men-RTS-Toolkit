#include "messagebox_event.h"
#include <new>
#include <stdio.h>

typedef void* (__cdecl* AllocMemOPNew_t)(size_t bytes);
static AllocMemOPNew_t AllocMemOPNew = (AllocMemOPNew_t)Memory::ScanAddress(0x4CFAB0);

typedef void* (__thiscall* MBEventCtor_t)(void* pThis, const char* ident, const char* caption);
static MBEventCtor_t MBEventCtor = (MBEventCtor_t)Memory::ScanAddress(0x4A6770);

typedef void* (__thiscall* MBEventCallbackCtor_t)(void* pThis, const char* ident, const char* caption, void* proc, unsigned int context);
static MBEventCallbackCtor_t MBEventCallbackCtor = (MBEventCallbackCtor_t)Memory::ScanAddress(0x4A6910);

MBEvent::MBEvent(const char* ident, const char* caption)
{
    MBEventCtor(this, ident, caption);
}

MBEvent::~MBEvent()
{
}

MBEventCallback* MBEventCallback::Create(const char* ident, const char* caption, Proc callbackProc, unsigned int callbackContext)
{
    void* mem = AllocMemOPNew(0x6C);
    if (!mem)
    {
        return nullptr;
    }

    MBEventCallbackCtor(mem, ident, caption, reinterpret_cast<void*>(callbackProc), callbackContext);
    MBEventCallback* obj = new (mem) MBEventCallback(ident, caption, callbackProc, callbackContext);
    void** vtablePtr = reinterpret_cast<void**>(obj);
    //printf("[MBEventCallback] Created at 0x%p, vtable at 0x%p\n", obj, *vtablePtr);

    return obj;
}

MBEventCallback::MBEventCallback(const char* ident, const char* caption, Proc callbackProc, unsigned int callbackContext) : MBEvent(ident, caption), m_proc(callbackProc), m_context(callbackContext)
{
}

void MBEventCallback::Process()
{
    if (m_proc)
    {
        //printf("hello 0x%p 0x%08\n", m_proc, m_proc);
        m_proc(0, m_context);
    }
}