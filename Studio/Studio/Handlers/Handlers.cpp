#include "Handlers.h"
#include "../GameFuncs/util/VarSys.h"
#include "../GameFuncs/coregame_interface/Client.h"
#include "../GameFuncs/coregame/TerrainData.h"
#include "../GameFuncs/graphics/Terrain.h"
#include "../Game/Game.h"

void teststuff()
{

}

void __fastcall Handlers::ClientDevCmdClientHandler(U32 value)
{
	//printf("s\n");
    printf("0x%08X\n", value);
    switch (value)
    {
		case 0x330F1664:
		{
			teststuff();
			break;
		}
        case 0x2185FED7:
        {
            /*if (mouse.sList.GetCount())
          {
            for (UnitObjList::Iterator i(&mouse.sList); *i; i++)
            {
              if ((*i)->Alive())
              {
                (**i)->SelfDestruct(TRUE);
              }
            }
          }
          else

          if (data.cInfo.gameWnd.Alive() && data.cInfo.o.map.Alive())
          {
            data.cInfo.o.map->SelfDestruct(TRUE);
          }

          break;*/

            DWORD mouseData = Client::GetMouse();
            if (!mouseData)
            {
                return;
            }

            DWORD* sList = (DWORD*)(mouseData + 0xA8);
			if (!sList)
			{
				return;
			}

            for (DWORD* it = *(DWORD**)sList; it && *it; it = *(DWORD**)it)
            {
                DWORD* unitPtr = it;
                if (!unitPtr)
                {
                    continue;
                }
            }
            Log::Client::Write("HELLO TOMMY!!!!3");
            teststuff();
            break;
        }
    }
}

void __fastcall Handlers::CmdClientHandler(U32 value)
{
    switch (value)
    {
        case 0xC55EC962:
        {

            break;
        }
    }
}

typedef int(__thiscall* VarItemInteger_t)(void*);
VarItemInteger_t VarItemInteger = (VarItemInteger_t)(Memory::ScanAddress(0x4E2FC0));
typedef float(__thiscall* VarItemFloat_t)(void* varItem);
VarItemFloat_t VarItemFloat = (VarItemFloat_t)(Memory::ScanAddress(0x4E30B0));

void __fastcall Handlers::StudioTerrainGenHandler(U32 value)
{
    switch (value)
    {
        case 0x4DC7D7DE:
        {
            //float minHeight = (float)terrainGenHeightMin;
            //float maxHeight = (float)terrainGenHeightMax;

            float minVal = VarItemFloat(g_terrainMinVarItem);
            float maxVal = VarItemFloat(g_terrainMaxVarItem);
            float minHeight = (float)minVal;
            float maxHeight = (float)maxVal;

            //void* minItem = VarSys::FindVarItem("studio.terraingen.min");
            //void* maxItem = VarSys::FindVarItem("studio.terraingen.max");
            //float minHeight = VarSys::GetFloat(minItem);
            //float maxHeight = VarSys::GetFloat(maxItem);
            
            //printf("min%f max%f\n", minHeight, maxHeight);

            //printf("min%f min2 %f\n", minHeight, minVal);
            //printf("max%f max2 %f\n", minHeight, maxVal);

            TerrainData::SessionStart();

            for (U32 x = 0; x < Terrain::CellWidth() + 1; x++)
            {
                for (U32 y = 0; y < Terrain::CellHeight() + 1; y++)
                {
                    float randomHeight = minHeight + (float)rand() / RAND_MAX * (maxHeight - minHeight);
                    TerrainData::SessionModifyHeight(x, y, randomHeight);
                }
            }

            TerrainData::SessionEnd();
            break;
        }
    }
}

//TODO, add a hook somewhere to call handlers again, after a "scene" change it deletes them
void Handlers::LoadHandlers()
{
    Internals::WaitForTrue(GameGod::IsInitialized());

    //Client development
    VarSys::RegisterHandler("client.development", ClientDevCmdClientHandler);
    VarSys::CreateCmd("client.development.setteam");
    VarSys::CreateCmd("client.development.blastthosebastards");
    VarSys::CreateCmd("client.development.money");
    VarSys::CreateCmd("client.development.tasks");

    VarSys::CreateCmd("client.development.directmodeon");
    VarSys::CreateCmd("client.development.directmodeoff");
    VarSys::CreateCmd("client.development.directturn");
    VarSys::CreateCmd("client.development.debug");
    
    //custom
	VarSys::CreateCmd("client.development.swag"); //case 0x330F1664: // "client.development.swag"

    //printf("handlers loaded\n");
    Log::Client::Write("[STUDIO DLL]: Handlers Loaded");
}