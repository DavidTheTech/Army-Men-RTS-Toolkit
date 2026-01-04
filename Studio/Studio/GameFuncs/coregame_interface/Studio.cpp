#include "Studio.h"

typedef int(*StudioProcess)();
typedef int(__cdecl* StudioInt)();
typedef void(__cdecl* StudioDN)();
typedef void(__cdecl* StudioPostInt)();

static StudioProcess StudioPrss = (StudioProcess)(Memory::ScanAddress(0x5D02B0));
static StudioInt StudioInitFn = (StudioInt)(Memory::ScanAddress(0x5D01E0));
static StudioDN StudioDoneFn = (StudioDN)(Memory::ScanAddress(0x5D0270));
static StudioPostInt StudioPostInitFn = (StudioPostInt)(Memory::ScanAddress(0x5D0260));

int Studio::Process()
{
    return StudioPrss();
}

int Studio::Init()
{
    return StudioInitFn();
}

void Studio::Done()
{
    StudioDoneFn();
}

void Studio::PostInit()
{
    StudioPostInitFn();
}
