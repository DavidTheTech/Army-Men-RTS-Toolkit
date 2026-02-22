#include "Testing.h"
#include "../Utils/Internals.h"
#include "../GameFuncs/game/GameGod.h"

typedef void* (__cdecl* AllocMemOPNew_t)(U32 size);
static AllocMemOPNew_t AllocMemOPNewFn = (AllocMemOPNew_t)(Memory::ScanAddress(0x005BBD6E));

static inline void* AllocMemOPNew(U32 size)
{
    return AllocMemOPNewFn(size);
}

typedef void* (__cdecl* Sub4037D0_t)(void);
static Sub4037D0_t Sub4037D0Fn = (Sub4037D0_t)(Memory::ScanAddress(0x004037D0));

static inline void* CallSub4037D0()
{
    return Sub4037D0Fn();
}

typedef void* (__thiscall* Sub5CCCD0_t)(void* dst, void* src);
static Sub5CCCD0_t Sub5CCCD0Fn = (Sub5CCCD0_t)(Memory::ScanAddress(0x005CCCD0));

static inline void* CallSub5CCCD0(void* dst, void* src)
{
    return Sub5CCCD0Fn(dst, src);
}

void Testing::Test()
{
    //*(char*)(0x68B818) = 0x73;
    
    memcpy((void*)0x70616D, "map", 3);
    //Internals::WaitForTrue(GameGod::IsInitialized());

    //Some debug func stuff??
    /*void* v1 = CallSub4037D0();
    void* v3 = AllocMemOPNew(0x528);
    if (!v3)
        return;

    void* v4 = CallSub5CCCD0(v3, v1);
    //dword_773194 = (U32)v4;#
    *(DWORD*)0x00773194 = (DWORD)v4;*/


}