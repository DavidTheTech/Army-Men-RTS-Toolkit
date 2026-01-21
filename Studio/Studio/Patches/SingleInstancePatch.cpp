#include "SingleInstancePatch.h"

void SingleInstancePatch::Apply()
{
    DWORD addr = Memory::ScanAddress(0x4B4792);

    DWORD old;
    VirtualProtect((LPVOID)addr, 1, PAGE_EXECUTE_READWRITE, &old);
    *(BYTE*)addr = 0x90;
    *(BYTE*)(addr + 0x01) = 0x90;
    VirtualProtect((LPVOID)addr, 1, old, &old);
    FlushInstructionCache(GetCurrentProcess(), (LPVOID)addr, 1);


    //tldr the game uses FindWindowA to check if an instance is already running
    //all we do is nop out the 2 bytes to ignore this check

    //IDA OUTPUT
    /*
        .text:004B4788                 push    edi             ; lpWindowName
        .text:004B4789                 push    esi             ; lpClassName
        .text:004B478A                 call    ds:__imp_FindWindowA
        .text:004B4790                 cmp     eax, edi
        .text:004B4792                 jz      short loc_4B47A6

        Hex
        004B4780  XX XX XX XX XX XX XX XX  57 56 FF 15 CC A2 66 00
        004B4790  3B C7 74 12 XX XX XX XX  XX XX XX XX XX XX XX XX
                        90^90
    */
}