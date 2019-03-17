//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Misc/Assert.h"
#include "Misc/Alignment.h"
#include "Memory/Allocator.h"
#include "Profiling/ProfilingUtility.h"

namespace Berserk
{

    Allocator::Allocator()
            : mFreeCalls(0),
              mAllocCalls(0),
              mReallocCalls(0),
              mTotalMemUsage(0)
    {
        /** Do actually nothing */
    }

    Allocator::~Allocator()
    {
        char buffer[20];

        /** Do actually nothing */
#if DEBUG
        printf("Allocator: total mem usage: %10s | alloc calls: %u | re-alloc calls: %u | free calls: %u \n",
               ProfilingUtility::print((uint32)getTotalMemoryUsage(), buffer),
               getAllocCalls(),
               getReallocCalls(),
               getFreeCalls()
        );
#endif
    }

    void* Allocator::memoryAllocate(size_t size)
    {
#ifdef VIRTUAL_MEMORY
        ALIGN(size);
        void* pointer = malloc(size);
        FAIL(pointer != nullptr, "Core: cannot malloc memory (size: %lu)", size);

        mAllocCalls += 1;
        mTotalMemUsage += size;

        return pointer;
#endif
    }

    void* Allocator::memoryCAllocate(size_t count, size_t size)
    {
#ifdef VIRTUAL_MEMORY
        ALIGN(size);
        void* pointer = calloc(count, size);
        FAIL(pointer != nullptr, "Core: cannot calloc memory (count: %lu, size: %lu)", count, size);

        mAllocCalls += 1;
        mTotalMemUsage += size * count;

        return pointer;
#endif
    }

    void* Allocator::memoryReallocate(void *old, size_t size)
    {
#ifdef VIRTUAL_MEMORY
        ALIGN(size);
        void* pointer = realloc(old, size);
        FAIL(pointer != nullptr, "Core: cannot realloc memory (pointer: %p, size: %lu)", old, size);

        mReallocCalls += 1;
        mTotalMemUsage += size;

        return pointer;
#endif
    }

    void Allocator::memoryFree(void *pointer)
    {
#ifdef VIRTUAL_MEMORY
        free(pointer);
        mFreeCalls += 1;
#endif
    }

    uint32 Allocator::getFreeCalls() const
    {
        return mFreeCalls;
    }

    uint32 Allocator::getAllocCalls() const
    {
        return mAllocCalls;
    }

    uint32 Allocator::getReallocCalls() const
    {
        return mReallocCalls;
    }

    uint64 Allocator::getTotalMemoryUsage() const
    {
        return mTotalMemUsage;
    }

    Allocator& Allocator::getSingleton()
    {
        static Allocator allocator;
        return allocator;
    }

}