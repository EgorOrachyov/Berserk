//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Public/Misc/Assert.h"
#include "Public/Misc/Alignment.h"
#include "Public/Memory/Allocator.h"

namespace Berserk
{

    Allocator::Allocator()
    {
        /** Do actually nothing */
    }

    Allocator::~Allocator()
    {
        /** Do actually nothing */
    }

    void* Allocator::memoryAllocate(size_t size)
    {
#ifdef VIRTUAL_MEMORY
        void* pointer = malloc(size);
        ASSERT(pointer != nullptr, "Core: cannot malloc memory (size: %lu)", size);
        return pointer;
#endif
    }

    void* Allocator::memoryCAllocate(size_t count, size_t size)
    {
#ifdef VIRTUAL_MEMORY
        void* pointer = calloc(count, size);
        ASSERT(pointer != nullptr, "Core: cannot calloc memory (count: %lu,size: %lu)", count, size);
        return pointer;
#endif
    }

    void Allocator::memoryFree(void *pointer)
    {
#ifdef VIRTUAL_MEMORY
        free(pointer);
#endif
    }

    Allocator& Allocator::getSingleton()
    {
        static Allocator allocator;
        return allocator;
    }

}