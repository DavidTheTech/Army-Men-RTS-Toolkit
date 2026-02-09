#include "Handlers.h"
#include "../GameFuncs/util/VarSys.h"

void __fastcall Handlers::CmdClientHandler(U32 value)
{
    printf("\n 0x%08X", value);
    switch (value)
    {
        case 0x2185FED7:
        {
            /*if (data.sList.GetCount())
          {
            for (UnitObjList::Iterator i(&data.sList); *i; i++)
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
            Log::Client::Write("HELLO TOMMY!!!!3");
            break;
        }
    }
}

void Handlers::LoadHandlers()
{
    Internals::WaitForTrue(GameGod::IsInitialized());

    VarSys::RegisterHandler("client.development", CmdClientHandler);
}
