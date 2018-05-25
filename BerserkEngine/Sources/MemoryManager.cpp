//
// Created by Egor Orachyov on 25.05.2018.
//

#include "../Core/MemoryManager.h"
#include "../Core/LogManager.h"

namespace Berserk
{

    MemoryManager::MemoryManager()
    {
        AddAllocatedBytes_Debug(sizeof(MemoryManager));
        AddAllocatedBytes_Debug(sizeof(LogManager));
    }

    MemoryManager::~MemoryManager()
    {

    }

    void MemoryManager::AddAllocatedBytes_Core(int32 bytes)
    {
        mCoreInfo.AddUsedMemory(bytes);
    }

    void MemoryManager::AddBufferedBytes_Core(int32 bytes)
    {
        mCoreInfo.AddBufferedMemory(bytes);
    }

    void MemoryManager::AddAllocatedBytes_Debug(int32 bytes)
    {
        mDebugInfo.AddUsedMemory(bytes);
    }

    void MemoryManager::AddBufferedBytes_Debug(int32 bytes)
    {
        mDebugInfo.AddBufferedMemory(bytes);
    }

    void MemoryManager::PushInfoIntoLog()
    {
        // info: test log pushing
        // todo: additional info + more readability

        char text[512];
        sprintf(text, "[Memory Usage] [bytes]\n Debug Memory : buffered %li | used %li | calls %i \n Core Memory  : buffered %li | used %li | calls %i \n Total memory: %li Total calls: %i",
                mDebugInfo.mBuffersMemory, mDebugInfo.mUsedMemory, mDebugInfo.mMemoryCalls,
                mCoreInfo.mBuffersMemory, mCoreInfo.mUsedMemory, mCoreInfo.mMemoryCalls,
                0L, mCoreInfo.mMemoryCalls + mDebugInfo.mMemoryCalls);
        globalLogManager.PushMessage(LMI_INFO, text);
    }

    MemoryManager globalMemoryManager;

} // namespace Berserk
