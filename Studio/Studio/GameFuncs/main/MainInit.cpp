#include "Main.h"
#include <stdio.h>

typedef HWND(__fastcall* CreateGameWindow_t)(LPCSTR lpWindowName, LPCSTR lpClassName);
static CreateGameWindow_t CreateGameWindowFn = (CreateGameWindow_t)(Memory::ScanAddress(0x4B4770));

HWND __fastcall Main::CreateGameWindow(LPCSTR lpWindowName, LPCSTR lpClassName)
{
    printf("swag money\n");
    return CreateGameWindowFn(lpWindowName, lpClassName);
}
