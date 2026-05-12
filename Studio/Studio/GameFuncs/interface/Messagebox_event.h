#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class MBEvent
{
public:
    const char* ident;
    void* caption;
    void* listNode;
    MBEvent(const char* ident, const char* caption);
    virtual ~MBEvent();
    virtual void Process() { }
};

class MBEventCallback : public MBEvent
{
public:
    typedef void (*Proc)(unsigned int event, unsigned int context);

    static MBEventCallback* Create(const char* ident, const char* caption, Proc callbackProc, unsigned int callbackContext = 0);

    virtual void Process() override;

private:
    MBEventCallback(const char* ident, const char* caption, Proc callbackProc, unsigned int callbackContext);

    Proc m_proc;
    unsigned int m_context;
};