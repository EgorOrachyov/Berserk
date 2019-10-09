//
// Created by Egor Orachyov on 19.03.2019.
//

#ifndef BERSERK_IALLOCATOR_H
#define BERSERK_IALLOCATOR_H

#include <HAL/Memory.h>

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

        /////////////// Static utility for implementation ///////////////

        /** @return True if alignment power of two */
        static inline bool isPowerOf2(uint32 alignment) {
            return ((alignment - 1) & alignment) == 0x0;
        }

        /** @return Aligned offset for ptr (by alignment - power of two!) */
        static inline uint32 align(void* ptr, uint32 alignment) {
            auto val = (uint64) ptr;
            return alignment - (val % alignment);
        }

        /** @return True, if ptr belongs to region origin of specified size */
        static inline bool belongs(uint8* ptr, uint8* origin, uint32 size) {
            return (origin <= ptr) && (ptr <= (origin + size));
        }

    };

} // namespace Berserk

#endif //BERSERK_IALLOCATOR_H