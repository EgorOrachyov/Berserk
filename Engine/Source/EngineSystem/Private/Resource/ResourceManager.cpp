//
// Created by Egor Orachyov on 2019-08-17.
//

#include <Resource/ResourceManager.h>

namespace Berserk
{

    ResourceManager::ResourceMemoryPool::ResourceMemoryPool(uint32 chunkSize, uint32 chunkCount,
                                                            AtomicInt &usage,
                                                            IAllocator &allocator)
    {

    }

    ResourceManager::ResourceMemoryPool::~ResourceMemoryPool() {

    }

    void *ResourceManager::ResourceMemoryPool::allocate(uint32 size, uint32 alignment) {
        return nullptr;
    }

    void ResourceManager::ResourceMemoryPool::free(void *pointer) {

    }

    uint32 ResourceManager::ResourceMemoryPool::getFreeCalls() const {
        return IAllocator::getFreeCalls();
    }

    uint32 ResourceManager::ResourceMemoryPool::getAllocateCalls() const {
        return IAllocator::getAllocateCalls();
    }

    uint64 ResourceManager::ResourceMemoryPool::getTotalMemoryUsage() const {
        return IAllocator::getTotalMemoryUsage();
    }
} // namespace Berserk