//
// Created by Egor Orachyov on 19.03.2019.
//

#ifndef BERSERK_IALLOCATOR_H
#define BERSERK_IALLOCATOR_H

#include <HAL/Platform.h>

namespace Berserk
{

    /** General engine allocator interface */
    class IAllocator {
    public:

        virtual ~IAllocator() = default;

        /** Allocates chosen size of continuous memory block */
        virtual void* malloc(uint32 size) = 0;

        /** Allocates chosen size of continuous memory block with desired alignment */
        virtual void* malloc(uint32 size, uint32 alignment) = 0;

        /** Free memory block */
        virtual void free(void* pointer) = 0;

    };

} // namespace Berserk

#endif //BERSERK_IALLOCATOR_H