#include "Hooks.h"

DWORD WINAPI Hooks::MinHookStuff(HINSTANCE hModule)
{
    MH_STATUS status = MH_Initialize();
    if (status != MH_OK)
    {
        std::string sStatus = MH_StatusToString(status);
        printf("Minhook init failed!\n");
        return 0;
    }

    /*if (MH_CreateHook(reinterpret_cast<void**>(hookCoreGameInit), &detourCoreGameInit, reinterpret_cast<void**>(&realCoreGameInit)) != MH_OK) return 1;
    if (MH_CreateHook(reinterpret_cast<void**>(hookHeapInit), &detourHeapInit, reinterpret_cast<void**>(&realHeapInit)) != MH_OK) return 1;
    if (MH_CreateHook(reinterpret_cast<void**>(hookInitBuckets), &detourInitBuckets, reinterpret_cast<void**>(&realInitBuckets)) != MH_OK) return 1;
    if (MH_CreateHook(reinterpret_cast<void**>(hookVidRenderBegin), &detourVidRenderBegin, reinterpret_cast<void**>(&realVidRenderBegin)) != MH_OK) return 1;
    if (MH_CreateHook(reinterpret_cast<void**>(hookVidRenderEnd), &detourVidRenderEnd, reinterpret_cast<void**>(&realVidRenderEnd)) != MH_OK) return 1;

    if (MH_EnableHook(reinterpret_cast<void**>(hookCoreGameInit)) != MH_OK) return 1;
    if (MH_EnableHook(reinterpret_cast<void**>(hookHeapInit)) != MH_OK) return 1;
    if (MH_EnableHook(reinterpret_cast<void**>(hookInitBuckets)) != MH_OK) return 1;*/
}