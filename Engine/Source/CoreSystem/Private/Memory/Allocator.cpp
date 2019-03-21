//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Misc/Assert.h"
#include "Misc/Alignment.h"
#include "Memory/Allocator.h"
#include "Profiling/ProfilingUtility.h"

namespace Berserk
{

    Allocator::Allocator() : IAllocator()
    {
        /** Do actually nothing */
    }

    Allocator::~Allocator()
    {
        /** Do actually nothing */
#if DEBUG
        char buffer[20];
        printf("======================================================================================================================= Alloc-calls: %u | Free-calls %u | Total: %10s\n",
               mAllocCalls, mFreeCalls, ProfilingUtility::print((uint32)getTotalMemoryUsage(), buffer));
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

        char buffer[20];
        printf("======================================================================================================================= Alloc-calls: %u | Free-calls %u | Total: %10s\n",
               mAllocCalls, mFreeCalls, ProfilingUtility::print((uint32)getTotalMemoryUsage(), buffer));

        return pointer;
#endif
    }

    void Allocator::free(void *pointer)
    {
#ifdef VIRTUAL_MEMORY
        ::free(pointer);
        mFreeCalls += 1;
#endif
    }

    Allocator& Allocator::getSingleton()
    {
        static Allocator allocator;
        return allocator;
    }

}