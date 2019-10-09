//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_MEMORY_H
#define BERSERK_MEMORY_H

#include <HAL/Platform.h>
#include <new>

namespace Berserk {

    /** Memory workflow class provider (wraps platform specific functions) */
    class Memory {
    public:

        /** Default memory address alignment */
        static const uint64 DEFAULT_ALIGNMENT = PlatformMemory::DEFAULT_ALIGNMENT;

        /** Memory size const */
        static const uint32 KiB = 1024;

        /** Memory size const */
        static const uint32 MiB = 1024 * KiB;

        /** Memory size const */
        static const uint32 GiB = 1024 * MiB;

    public:

        /** Allocates size bytes of memory */
        static inline void* malloc(uint64 size) {
            return PlatformMemory::malloc(size);
        }

        /** Allocates size bytes of aligned memory */
        static inline void* malloc(uint64 size, uint64 alignment) {
            return PlatformMemory::malloc(size, alignment);
        }

        /** Allocates reallocates original buffer to new buffer of size bytes */
        static inline void* realloc(void* original, uint64 size) {
            return PlatformMemory::realloc(original, size);
        }

        /** Free buffer via pointer and set pointer to null */
        static inline void free(void* pointer) {
            PlatformMemory::free(pointer);
        }

        /** Copy size bytes form source to destination buffer */
        static inline void copy(void* destination, void* source, uint64 size) {
            PlatformMemory::copy(destination, source, size);
        }

        /** Set mem source buffer to zero */
        static inline void zero(void* source, uint64 size) {
            PlatformMemory::zero(source, size);
        }

    };

}

#endif //BERSERK_MEMORY_H