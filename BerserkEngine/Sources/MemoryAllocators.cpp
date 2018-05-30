//
// Created by Egor Orachyov on 25.05.2018.
//

#include "../Core/Common.h"
#include "../Core/Assert.h"
#include "../Core/MemoryAllocators.h"
#include "../Core/Profiling/MemoryProfiler.h"

namespace Berserk
{

    void* mem_alloc(uint32 size)
    {
        #ifdef VIRTUAL_MEMORY
            void* pointer = malloc(size);
        #endif

        ASSERT(pointer, "Cannot allocate memory");

        return pointer;
    }

    void* mem_calloc(uint32 count, uint32 size)
    {
        #ifdef VIRTUAL_MEMORY
            void* pointer = calloc(count, size);
        #endif

        ASSERT(pointer, "Cannot allocate memory");    
        
        return pointer;
    }

    void* mem_realloc(void *oldPointer, uint32 newSize)
    {
        #ifdef VIRTUAL_MEMORY
            void* pointer = realloc(oldPointer, newSize);
        #endif

        ASSERT(pointer, "Cannot allocate memory");    
        
        return pointer;
    }

    void* mem_alloc(uint32 size, uint8 alignment)
    {
        ASSERT((alignment - 1) & alignment == 0, "Alignment is not a power of 2");

        #ifdef VIRTUAL_MEMORY
            void* pointer = malloc(size);
        #endif
        
        ASSERT(pointer, "Cannot allocate memory");
        
        return pointer;
    }

    void* mem_calloc(uint32 count, uint32 size, uint8 alignment)
    {
        ASSERT((alignment - 1) & alignment == 0, "Alignment is not a power of 2");

        #ifdef VIRTUAL_MEMORY
            void* pointer = calloc(count, size);
        #endif
        
        ASSERT(pointer, "Cannot allocate memory");

        return pointer;
    }

    void* mem_realloc(void *oldPointer, uint32 newSize, uint8 alignment)
    {
        ASSERT((alignment - 1) & alignment == 0, "Alignment is not a power of 2");

        #ifdef VIRTUAL_MEMORY
            void* pointer = realloc(oldPointer, newSize);
        #endif

        ASSERT(pointer, "Cannot allocate memory");
        
        return pointer;
    }

    void mem_free(void *pointer)
    {
        ASSERT(pointer, "Attempt to free NULL pointer");

        #ifdef VIRTUAL_MEMORY
            free(pointer);
        #endif
    }

} // namespace Berserk
