#include "ConsoleEditPatch.h"

#define ORIGINAL_JZ_ADDRESS 0x4A4C8C
#define LOC_4A542D         0x4A542D

typedef void* (__stdcall* AllocMem2_t)(DWORD dwBytes);
static AllocMem2_t AllocMem2 = (AllocMem2_t)(Memory::ScanAddress(0x4CFAB0));

typedef void(__stdcall* ConsoleEdit__ConsoleEdit_t)(DWORD* ctrl, DWORD* a);
static ConsoleEdit__ConsoleEdit_t ConsoleEdit__ConsoleEdit = (ConsoleEdit__ConsoleEdit_t)(Memory::ScanAddress(0x652EB0));

static void __declspec(naked) ConsoleEditTweak()
{
    __asm
    {
        push    0x2F8
        call    AllocMem2
        add     esp, 4

        mov[ebp + 0x10], eax
        test    eax, eax
        jz      loc_return

        push    esi
        mov     ecx, eax
        call    ConsoleEdit__ConsoleEdit
        pop     esi

        mov     ecx, [ebp + 0x0C]
        mov     dword ptr fs : [0] , ecx
        mov     esp, ebp
        pop     ebp
        retn    4

        loc_return :
        mov     eax, LOC_4A542D
        jmp     eax
    }
}

void ConsoleEditPatch::Apply()
{
    DWORD oldProtect;

    DWORD addr = Memory::ScanAddress(ORIGINAL_JZ_ADDRESS);

    VirtualProtect((LPVOID)addr, 6, PAGE_EXECUTE_READWRITE, &oldProtect);

    BYTE* patch = (BYTE*)addr;
    patch[0] = 0x0F;
    patch[1] = 0x84;

    DWORD target = (DWORD)&ConsoleEditTweak;
    DWORD offset = target - (addr + 6);
    *(DWORD*)(patch + 2) = offset;

    VirtualProtect((LPVOID)addr, 6, oldProtect, &oldProtect);
}
