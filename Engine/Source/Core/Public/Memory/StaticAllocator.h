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

#ifndef BERSERK_STATICALLOCATOR_H
#define BERSERK_STATICALLOCATOR_H

#include <HAL/Memory.h>
#include <Misc/Error.h>

namespace Berserk {

    /**
     * Linear allocator with static buffer storage of fixed size
     * @tparam CAPACITY Allocated memory buffer size [in bytes]
     */
    template <uint32 CAPACITY = Memory::KiB>
    class StaticAllocator : public IAllocator {
    public:

        static_assert(CAPACITY > 0, "Capacity must be more than 0");

        StaticAllocator() = default;
        ~StaticAllocator() override {
            clean();
        }

        /** @copydoc IAllocator::malloc() */
        void *malloc(uint32 size) override {
            uint32 newSize = mAllocated + size;
            DEV_ERROR_CONDITION(newSize <= CAPACITY, "Cannot allocate memory");

            void* mem = &mBuffer[mAllocated];
            mAllocated = newSize;
            return mem;
        }

        /** @copydoc IAllocator::malloc() */
        void *malloc(uint32 size, uint32 alignment) override {
            DEV_ERROR_CONDITION(Memory::isPowerOf2(alignment), "Alignment must be power of two");
            uint32 offset = Memory::align(&mBuffer[mAllocated], alignment);
            uint32 newSize = mAllocated + size + offset;
            DEV_ERROR_CONDITION(newSize <= CAPACITY, "Cannot allocate memory");

            void* mem = &mBuffer[mAllocated + offset];
            mAllocated = newSize;
            return mem;
        }

        /** @copydoc IAllocator::free() */
        void free(void *pointer) override {
            /** Cannot free. only clean */
        }

        /**
         * Clean buffer.
         * @warning All allocations could be invalidated
         */
        void clean() {
            mAllocated = 0;
        }

        /** @return Allocated memory size */
        uint32 getMemoryUsage() const {
            return mAllocated;
        }

    private:

        /** Currently allocated bytes count */
        uint32 mAllocated = 0;

        /** Memory buffer */
        uint8 mBuffer[CAPACITY];

    };

}

#endif //BERSERK_STATICALLOCATOR_H
