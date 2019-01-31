//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Public/Misc/Assert.h"
#include "Public/Misc/Alignment.h"
#include "Public/Misc/Compilation.h"
#include "Public/Memory/Allocator.h"

namespace Berserk
{

    Allocator::Allocator()
            : mFreeCalls(0),
              mAllocCalls(0),
              mTotalMemUsage(0)
    {
        /** Do actually nothing */
    }

    Allocator::~Allocator()
    {
        /** Do actually nothing */
#if DEBUG
        printf("Allocator: total mem usage: %lu | alloc calls: %u | free calls: %u \n",
               getTotalMemoryUsage(),
               getAllocCalls(),
               getFreeCalls()
        );
#endif
    }

    void* Allocator::memoryAllocate(size_t size)
    {
#ifdef VIRTUAL_MEMORY
        ALIGN(size);
        void* pointer = malloc(size);
        ASSERT(pointer != nullptr, "Core: cannot malloc memory (size: %lu)", size);

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
        ASSERT(pointer != nullptr, "Core: cannot calloc memory (count: %lu, size: %lu)", count, size);

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
        ASSERT(pointer != nullptr, "Core: cannot realloc memory (pointer: %p, size: %lu)", old, size);

        mAllocCalls += 1;
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