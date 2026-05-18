#include "studio_brush_terrainGen.h"
#include <stdio.h>
#include "../GameFuncs/coregame_interface/Studio_brush_area.h"

namespace Studio
{
    namespace Brush
    {
        template<typename T>
        static void* GetFuncPtr(T f)
        {
            union { T f; void* p; } u;
            u.f = f;
            return u.p;
        }

        static void* const WATER_VTABLE = (void*)0x670FF0;

        //water vtable virt funcs
        static void* const ORIG_NOTIFYVAR = (void*)0x5D6DC0;
        static void* const ORIG_CURSOR = (void*)0x5BC4B0;
        static void* const ORIG_UNKNOWN = (void*)0x5BC4D0;

        void* TerrainGen::customVtable[6];
        bool vtableInit = false;

        TerrainGen::TerrainGen(void* meem, const char* name)
        {
            if (!vtableInit)
            {
                memcpy(customVtable, WATER_VTABLE, sizeof(customVtable));
                customVtable[0] = GetFuncPtr(&TerrainGen::Notification);
                vtableInit = true;
            }

            Studio::Brush::AreaBase::Constructor(meem, name);

            *(void**)meem = customVtable;
        }

        TerrainGen::~TerrainGen()
        {
        }

        void TerrainGen::Notification(U32 crc, DWORD* e)
        {
            switch (crc)
            {
                /*case 0x8B9FFA39: // "System::PostDraw"
                {
                    return;
                }
                case 0xC3C52EA3: // "System::PreDraw"
                {
                    return;
                }
                case 0x8C6EE5DF: // "System::PostEventPoll"
                {
                    return;
                }
                case 0x44D1C3B4: // "System::PostCycleInfoPoll"
                {
                    return;
                }
                case 0x457659C0: // "System::PostDeletionPoll"
                {
                    return;
                }
                case 0x90486A58: // "System::PreEventPoll"
                {
                    return;
                }*/

                case 0x6153E7A4: // "Brush::TerrainGen::Message::Apply"
                {

                    return;
                }

                case 0x985B3F49: // "Command::Select"
                {

                    Studio::Brush::AreaBase::Notification(this, crc, e);
                    return;
                }
            }
            Studio::Brush::AreaBase::Notification(this, crc, e);
        }

        Bool TerrainGen::HasProperty(U32 property)
        {
            return FALSE;
        }

        void TerrainGen::NotifyVar(void* var)
        {
            //this func actually doesnt go to anything, its a nullsub
            typedef void(__thiscall* NotifyVar_t)(void*, void*);
            static NotifyVar_t origNotifyVar = (NotifyVar_t)ORIG_NOTIFYVAR;
            origNotifyVar(this, var);
        }
    }
}