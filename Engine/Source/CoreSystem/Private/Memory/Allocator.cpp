//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Misc/Assert.h"
#include "Misc/Alignment.h"
#include "Memory/Allocator.h"
#include "Profiling/ProfilingUtility.h"

namespace Berserk
{

    Allocator::Allocator() : mFreeCalls(0),
                             mAllocCalls(0),
                             mTotalMemUsage(0)
    {
        /** Do actually nothing */
    }

    Allocator::~Allocator()
    {
        char buffer[20];

        /** Do actually nothing */
#if DEBUG
        printf("Allocator: total mem usage: %10s | alloc calls: %u | free calls: %u \n",
               ProfilingUtility::print((uint32)getTotalMemoryUsage(), buffer),
               getAllocateCalls(),
               getFreeCalls()
        );
#endif
    }

    void* Allocator::allocate(uint32 size)
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

    void Allocator::free(void *pointer)
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

    uint32 Allocator::getAllocateCalls() const
    {
        return mAllocCalls;
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