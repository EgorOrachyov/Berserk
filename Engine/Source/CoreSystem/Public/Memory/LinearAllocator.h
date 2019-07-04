//
// Created by Egor Orachyov on 03.07.2019.
//

#ifndef BERSERK_LINEARALLOCATOR_H
#define BERSERK_LINEARALLOCATOR_H

#include <Memory/Allocator.h>
#include <Misc/Buffers.h>

namespace Berserk
{

    /**
     * Allocates memory from single continual memory buffer.
     * Cannot free memory via free call - only clear - to clear
     * all the used memory and begin allocate from empty buffer.
     *
     * @example LinearAllocator allocator;
     *          allocator.allocate();
     *          allocator.allocate();
     *          allocator.allocate();
     *          ...
     *          allocator.clear();
     *          this function set buffer usage to 0.
     */
    class MEMORY_API LinearAllocator : public IAllocator
    {
    public:

        /** Default internal memory buffer size */
        static const uint32 DEFAULT_BUFFER_SIZE = 10 * KiB;

    public:

        /**
         * Initialize allocator and preallocate need memory buffer
         * @param bufferSize Size of the buffer to preallocate
         * @param allocator Allocator for internal usage
         */
        LinearAllocator(uint32 bufferSize = DEFAULT_BUFFER_SIZE, IAllocator& allocator = Allocator::get());

        ~LinearAllocator() override;

        /** @copydoc IAllocator::allocate() */
        void *allocate(uint32 size) override;

        /** @copydoc IAllocator::allocate() */
        void *allocate(uint32 size, uint32 alignment) override;

        /** @copydoc IAllocator::free() */
        void free(void *pointer) override;

        /**
         * Resets or frees all the allocated data by allocator and
         * marks internal buffer as totally free
         *
         * @warning All the allocated data will be lost, therefore this
         *          method should be called only when all the allocated
         *          chunks are not used by systems elements (User should
         *          explicitly ensure this process)
         */
        void clear();

        /** @copydoc IAllocator::getFreeCalls() */
        uint32 getFreeCalls() const override { return mAllocateCalls; }

        /** @copydoc IAllocator::getAllocateCalls() */
        uint32 getAllocateCalls() const override { return mFreeCalls; }

        /** @copydoc IAllocator::getTotalMemoryUsage() */
        uint64 getTotalMemoryUsage() const override { return mTotalMemoryUsage; }

    private:

        /** Checks, whether can allocate this memory block */
        bool canAllocate(uint32 size) const;

    private:

        IAllocator& mAllocator;

        uint8* mBuffer = nullptr;
        uint32 mCurrentUsage = 0;
        uint32 mBufferSize = 0;

        uint32 mAllocateCalls = 0;
        uint32 mFreeCalls = 0;
        uint64 mTotalMemoryUsage = 0;

    };

} // namespace Berserk

#endif //BERSERK_LINEARALLOCATOR_H