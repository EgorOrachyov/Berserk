//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_MEMORYMANAGER_H
#define BERSERKENGINE_MEMORYMANAGER_H

#include "Types.h"

namespace Berserk
{
    class MemoryManager
    {
    private:

        typedef struct SystemInfo
        {
        public:

            SystemInfo()
            {
                mBuffersMemory = 0;
                mUsedMemory = 0;
                mMemoryCalls = 0;
            }

            void AddUsedMemory(int32 bytes)
            {
                mUsedMemory += bytes;
                mMemoryCalls += 1;
            }

            void AddBufferedMemory(int32 bytes)
            {
                mBuffersMemory += bytes;
                mMemoryCalls += 1;
            }

            int64 mBuffersMemory;
            int64 mUsedMemory;
            int32 mMemoryCalls;

        } SystemInfo;

    public:

        MemoryManager();

        ~MemoryManager();

        void AddAllocatedBytes_Core(int32 bytes);

        void AddBufferedBytes_Core(int32 bytes);

        void AddAllocatedBytes_Debug(int32 bytes);

        void AddBufferedBytes_Debug(int32 bytes);

        void PushInfoIntoLog();

    private:

        SystemInfo mCoreInfo;
        SystemInfo mDebugInfo;

    };

    /**
     * Global Singleton Memory Manager
     */
    extern MemoryManager globalMemoryManager;

} // namespace Berserk

#endif //BERSERKENGINE_MEMORYMANAGER_H
