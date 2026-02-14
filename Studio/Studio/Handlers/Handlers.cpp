#include "Handlers.h"
#include "../GameFuncs/util/VarSys.h"
#include "../GameFuncs/coregame_interface/Client.h"

void teststuff()
{

}

void __fastcall Handlers::CmdClientHandler(U32 value)
{
	printf("s\n");
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

//TODO, add a hook somewhere to call handlers again, after a "scene" change it deletes them
void Handlers::LoadHandlers()
{
    Internals::WaitForTrue(GameGod::IsInitialized());

    VarSys::RegisterHandler("client.development", CmdClientHandler);

	VarSys::CreateCmd("client.development.blastthosebastards");
	VarSys::CreateCmd("client.development.swag"); //case 0x330F1664: // "client.development.swag"
	printf("ss\n");
}