//
// Created by Egor Orachyov on 2019-08-10.
//

#ifndef BERSERK_RESOURCEMANAGER_H
#define BERSERK_RESOURCEMANAGER_H

#include <Resource/Resource.h>
#include <Containers/THashMap.h>

namespace Berserk
{

    /**
     * Generic resource manager class. Manager single kind of resource
     * (construction and life-cycle).
     *
     * @note Resource manager must construct, allocate, and store resource
     *       in the memory. Resource cache will only access resource manager
     *       and get its resource via name/UUID.
     *
     * @note Each resource manager must follow memory budget and allocation
     *       policy. Store/cache allocated resources.
     *
     * @note All the resource managers will be handled by resource cache.
     *       Each resource will be accessible only via resource cache.
     *
     * @note Thread-safe. Each resource manager contains internal
     *       mutex to manage operations.
     */
    class ENGINE_API ResourceManager : public Allocatable
    {
    public:

        virtual ~ResourceManager() = default;

        /**
         * @note If resource manager needs to load resource and there is not enough
         *       memory in the pool (specified via this budget), it will fail to
         *       load new resource and silently return default resource [or throw an error].
         *
         * @return Size of the memory available for loading resources [in bytes]
         */
        uint32 getMemoryBudget() const { return mMemoryBudget; }

        /** @return Current memory usage for allocated resources [in bytes] */
        uint32 getMemoryUsage() const { return mResourcePool.getMemoryUsage(); }

    protected:

        /** Thread safe resource manger internal pool */
        class ResourceMemoryPool final : public IAllocator
        {
        public:

            ResourceMemoryPool(uint32 chunkSize, uint32 chunkCount, IAllocator& allocator = Allocator::get());

            ~ResourceMemoryPool() override = default;

            /** @copydoc IAllocator::allocate() */
            void* allocate(uint32 size) override { allocate(size, MEMORY_ALIGNMENT); };

            /** @copydoc IAllocator::allocate() */
            void *allocate(uint32 size, uint32 alignment) override;

            /** @copydoc IAllocator::free() */
            void free(void *pointer) override;

            /** @copydoc IAllocator::getFreeCalls() */
            uint32 getFreeCalls() const override;

            /** @copydoc IAllocator::getAllocateCalls() */
            uint32 getAllocateCalls() const override;

            /** @return Currently allocated memory [in bytes] */
            uint32 getMemoryUsage() const;

            /** @copydoc IAllocator::getTotalMemoryUsage() */
            uint64 getTotalMemoryUsage() const override;

        private:

            AtomicInt mMemoryUsage;
            mutable Mutex mMutex;
            PoolAllocator mPool;

        };

    public:

        /** Internal manager mutex for thread-safety */
        Mutex mMutex;

        /** How mach memory could use for allocating new resources */
        uint32 mMemoryBudget;

        /** Memory pool to allocate resources */
        typedef ResourceMemoryPool ResourcePool;
        ResourcePool mResourcePool;

        /** Pool to allocate nodes inside resource map */
        PoolAllocator mResourceMapPool;

        /** Maps UUID to resource */
        typedef THashMap<UUID,ResourceRef> ResourceMap;
        ResourceMap mResourceMap;

    };

} // namespace Berserk

#endif //BERSERK_RESOURCEMANAGER_H