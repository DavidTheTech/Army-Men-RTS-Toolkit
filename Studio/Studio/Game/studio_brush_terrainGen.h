#include "..\Memory\Memory.h"

namespace Studio
{
    namespace Brush
    {
        class TerrainGen// : public AreaBase
        {
        public:

            TerrainGen(void* meem, const char* name);
            ~TerrainGen();

            void Notification(U32 crc, DWORD* e);
            Bool HasProperty(U32 property);
            void NotifyVar(void* var);
            void ScalarDelete(unsigned int flags);
            void DestructorWrapper();
            void* Cursor();
            void* Unknown();

        private:
            static void* customVtable[6];
        };
    }
}