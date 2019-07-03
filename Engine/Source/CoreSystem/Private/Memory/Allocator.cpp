//
// Created by Egor Orachyov on 24.01.2019.
//

#include <Misc/AssertDev.h>
#include <Misc/Alignment.h>
#include <Memory/Allocator.h>
#include <Misc/Compilation.h>

namespace Berserk
{

    Allocator::Allocator()
    {
        /** Do actually nothing */
    }

    Allocator::~Allocator()
    {
        /** Do actually nothing */
        char buffer[20];
#if DEBUG
        printf("Allocator(Global): [allocation calls: %u] [free calls: %u] [total memory usage: %s]\n",
               mAllocCalls, mFreeCalls, Printer::print((uint32)getTotalMemoryUsage(), buffer));
#endif

        assertion_dev_msg(mAllocCalls == mFreeCalls,
                          "Allocator(Global): [allocation calls: %u] [free calls: %u] [total memory usage: %s]\n",
                          mAllocCalls, mFreeCalls, Printer::print((uint32)getTotalMemoryUsage(), buffer));
    }

    void* Allocator::allocate(uint32 size)
    {
        return allocate(size, MEMORY_ALIGNMENT);
    }

    void* Allocator::allocate(uint32 size, uint32 alignment)
    {
        CriticalSection section(mMutex);

#ifdef VIRTUAL_MEMORY
        ALIGNMENT_PARAM(size, alignment);
        void* pointer = malloc(size);
        assertion_dev_msg(pointer != nullptr, "Core: cannot malloc memory (size: %lu)", size);

        mAllocCalls += 1;
        mTotalMemUsage += size;

#if PROFILE_SYSTEM_ALLOCATOR
        char buffer[20];
        char allocated[20];
        printf("Allocator(Global): ====================================================================================="
                       " Allocate: %10s | Alloc-calls: %u | Free-calls %u | Total: %10s\n",
               Printer::print(size, allocated),
               mAllocCalls, mFreeCalls,
               Printer::print((uint32)getTotalMemoryUsage(), buffer));
#endif

        return pointer;
#endif
    }

    void Allocator::free(void *pointer)
    {
        CriticalSection section(mMutex);

#ifdef VIRTUAL_MEMORY
        ::free(pointer);
        mFreeCalls += 1;
#endif
    }

    Allocator& Allocator::get()
    {
        static Allocator allocator;
        return allocator;
    }

}