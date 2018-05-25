//
// Created by Egor Orachyov on 25.05.2018.
//

#include "../Core/Common.h"
#include "../Core/Assert.h"
#include "../Core/MemoryAllocators.h"
#include "../Core/MemoryManager.h"

namespace Berserk
{
    void* Alloc(uint32 size)
    {
        #ifdef VIRTUAL_MEMORY
            void* pointer = malloc(size);
        #endif

        ASSERT(pointer, "Cannot allocate memory");    
        
        return pointer;
    }

    void* CAlloc(uint32 count, uint32 size)
    {
        #ifdef VIRTUAL_MEMORY
            void* pointer = calloc(count, size);
        #endif

        ASSERT(pointer, "Cannot allocate memory");    
        
        return pointer;
    }

    void* ReAlloc(void* oldPointer, uint32 newSize)
    {
        #ifdef VIRTUAL_MEMORY
            void* pointer = realloc(oldPointer, newSize);
        #endif

        ASSERT(pointer, "Cannot allocate memory");    
        
        return pointer;
    }

    void* Alloc_aligned(uint32 size, uint8 alignment)
    {
        ASSERT((alignment - 1) & alignment == 0, "Alignment is not a power of 2");

        #ifdef VIRTUAL_MEMORY
            void* pointer = malloc(size);
        #endif
        
        ASSERT(pointer, "Cannot allocate memory");
        
        return pointer;
    }

    void* CAlloc_aligned(uint32 count, uint32 size, uint8 alignment)
    {
        ASSERT((alignment - 1) & alignment == 0, "Alignment is not a power of 2");

        #ifdef VIRTUAL_MEMORY
            void* pointer = calloc(count, size);
        #endif
        
        ASSERT(pointer, "Cannot allocate memory");

        return pointer;
    }

    void* ReAlloc_aligned(void* oldPointer, uint32 newSize, uint8 alignment)
    {
        ASSERT((alignment - 1) & alignment == 0, "Alignment is not a power of 2");

        #ifdef VIRTUAL_MEMORY
            void* pointer = realloc(oldPointer, newSize);
        #endif
        
        return pointer;
    }

} // namespace Berserk
