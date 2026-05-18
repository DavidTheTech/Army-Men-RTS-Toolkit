#include "Studio_brush_area.h"

typedef void* (__thiscall* StudioBrushAreaBase_Constructor_t)(void* thisPtr, const char* name);
static StudioBrushAreaBase_Constructor_t AreaBase_Constructor_Fn = (StudioBrushAreaBase_Constructor_t)Memory::ScanAddress(0x5E70D0);

typedef void(__thiscall* StudioBrushAreaBaseNotification_t)(void*, U32, void*);
static StudioBrushAreaBaseNotification_t AreaBase_Notification_Fn = (StudioBrushAreaBaseNotification_t)Memory::ScanAddress(0x5E7170);

void Studio::Brush::AreaBase::Constructor(void* thisPtr, const char* name)
{
	AreaBase_Constructor_Fn(thisPtr, name);
}

void Studio::Brush::AreaBase::Notification(void* thisPtr, U32 crc, DWORD* e)
{
	AreaBase_Notification_Fn(thisPtr, crc, e);
}