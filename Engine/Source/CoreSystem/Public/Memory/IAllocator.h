//
// Created by Egor Orachyov on 19.03.2019.
//

#ifndef BERSERK_IALLOCATOR_H
#define BERSERK_IALLOCATOR_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk
{

    class MEMORY_API IAllocator
    {
    public:

        virtual ~IAllocator() = default;

        /** Allocates chosen size of continuous memory block */
        virtual void* allocate(uint32 size) = 0;

        /** Free memory block */
        virtual void free(void *pointer) = 0;

        /** @return Total number of memoryFree calls in the engine [in bytes] */
        uint32 getFreeCalls() const { return mFreeCalls; }

        /** @return Total number of memoryAllocate and memoryCAllocate in the engine [in bytes] */
        uint32 getAllocateCalls() const { return mAllocCalls; }

        /** @return Total memory usage for the whole time of engine working [in bytes] */
        uint64 getTotalMemoryUsage() const { return mTotalMemUsage; }

    protected:

        uint32 mFreeCalls = 0;      // Total number of free calls in the engine [in bytes]
        uint32 mAllocCalls = 0;     // Total number of allocate and memoryCAllocate in the engine [in bytes]
        uint64 mTotalMemUsage = 0;  // Total number of allocated mem (this mem actually could be freed)

    };

} // namespace Berserk

#endif //BERSERK_IALLOCATOR_H