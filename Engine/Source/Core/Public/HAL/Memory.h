/**********************************************************************************/
/* This file is part of:                                                          */
/* Berserk Engine project                                                         */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018-2019 Egor Orachyov                                          */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_MEMORY_H
#define BERSERK_MEMORY_H

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
        static inline void copy(void* destination, const void* source, uint64 size) {
            PlatformMemory::copy(destination, source, size);
        }

        /** Set mem source buffer to zero */
        static inline void zero(void* source, uint64 size) {
            PlatformMemory::zero(source, size);
        }

        /////////////// Static utility ///////////////

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

}

#endif //BERSERK_MEMORY_H