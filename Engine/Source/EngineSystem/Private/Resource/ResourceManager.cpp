//
// Created by Egor Orachyov on 2019-08-17.
//

#include <Resource/ResourceManager.h>

namespace Berserk
{

    ResourceManager::ResourceMemoryPool::ResourceMemoryPool(Berserk::uint32 chunkSize, Berserk::uint32 chunkCount,
                                                            Berserk::IAllocator &allocator)
                                                            : mMemoryUsage(0),
                                                              mPool(chunkSize, chunkCount)
    {

    }

    void *ResourceManager::ResourceMemoryPool::allocate(uint32 size, uint32 alignment)
    {
        CriticalSection section(mMutex);
        void* memory = mPool.allocate(size, alignment);
        return memory;
    }

    void ResourceManager::ResourceMemoryPool::free(void *pointer)
    {
        CriticalSection section(mMutex);
        mPool.free(pointer);
    }

    uint32 ResourceManager::ResourceMemoryPool::getFreeCalls() const
    {
        CriticalSection section(mMutex);
        return mPool.getFreeCalls();
    }

    uint32 ResourceManager::ResourceMemoryPool::getAllocateCalls() const
    {
        CriticalSection section(mMutex);
        return mPool.getAllocateCalls();
    }

    uint32 ResourceManager::ResourceMemoryPool::getMemoryUsage() const
    {
        return mMemoryUsage.load(std::memory_order_relaxed);
    }

    uint64 ResourceManager::ResourceMemoryPool::getTotalMemoryUsage() const
    {
        CriticalSection section(mMutex);
        return mPool.getTotalMemoryUsage();

    }

} // namespace Berserk