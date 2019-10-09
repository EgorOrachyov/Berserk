//
// Created by Egor Orachyov on 2019-10-09.
//

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
            DEV_ERROR_CONDITION(isPowerOf2(alignment), "Alignment must be power of two");
            uint32 offset = align(&mBuffer[mAllocated], alignment);
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
        uint32 getMemomyUsage() const {
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
