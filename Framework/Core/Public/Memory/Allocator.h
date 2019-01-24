//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_ALLOCATOR_H
#define BERSERK_ALLOCATOR_H

#include "Public/Misc/Include.h"
#include "Public/Misc/UsageDescriptors.h"

namespace Berserk
{

    /**
     * @brief Allocator
     *
     * Wrapper for platform specific OS system calls
     * for acquiring memory from OS for engine specific
     * sub-systems and allocators
     */
    class MEMORY_API Allocator {

    public:

        Allocator();
        ~Allocator();

        void* memoryAllocate(size_t size);

        void* memoryCAllocate(size_t count, size_t size);

        void  memoryFree(void* pointer);

        static Allocator& getSingleton();

    };

}

#endif //BERSERK_ALLOCATOR_H