//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_STACKALLOCATOR_H
#define BERSERKENGINE_STACKALLOCATOR_H

#include "../Types.h"
#include "../Common.h"
#include "../Assert.h"
#include "../MemoryAllocators.h"
#include "../UsageDescriptors.h"

namespace Berserk
{

    /**
     * @brief Memory Allocation
     *
     * Basic stack allocator which gives memory from pre-allocated fixed size
     * buffer. Allows set marker and free buffer for const operation time.
     * Does not allow random free of allocated memory
     */
    class MEM_API StackAllocator
    {
    public:

        StackAllocator()
        {
            mSize = 0;
            mTop = 0;
            mMarker = 0;
            mBuffer = NULL;
        }

        ~StackAllocator()
        {
            if (mBuffer)
            { mem_free(mBuffer); }
        }

        /**
         * Init stack and pre-allocate buffer
         *
         * @param size Size of stack buffer in bytes (cannot be less than 64)
         */
        inline void Init(uint32 size = 64)
        {
            ASSERT(size >= 64, "Buffer size cannot be less than 64 bytes");

            mTop = 0;
            mMarker = 0;
            mSize = size;

            ASSERT(!mBuffer, "Buffer should be NULL before init");

            mBuffer = mem_alloc(mSize);
        }

        /**
         * Free internal buffer and set stack to initial values (0) (notice: all the
         * allocated variables by this stack will be lost)
         */
        inline void Reset()
        {
            if (mBuffer)
            { mem_free(mBuffer); }

            mSize = 0;
            mTop = 0;
            mMarker = 0;
            mBuffer = NULL;
        }

        /**
         * Reset stack and init it with new setting (@see Init @see Reset)
         *
         * @param size Size of stack buffer in bytes (cannot be less than 64)
         */
        inline void ReInit(uint32 size = 64)
        {
            Reset();
            Init(size);
        }

        /**
         * Allocate memory for desired size block or crash engine if it does not
         * another free block (uses standard alignment MEMORY_ALIGNMENT)
         *
         * @param size Size of memory block to be allocated
         * @return Pointer to free block
         */
        inline void* Alloc(uint32 size)
        {
            size = size + (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));
            ASSERT(mTop + size <= mSize, "Buffer is full, cannot allocate memory");

            uint32 offset = mTop + size;
            void* ptr = (void*)((int8*)mBuffer + offset);
            mTop = offset;

            return ptr;
        }

        /**
         * Allocate memory for set of blocks or crash engine if it does not
         * another free block (uses standard alignment MEMORY_ALIGNMENT)
         *
         * @param count Number of blocks to be allocated
         * @param size Size of one block
         * @return Pointer to the first block
         */
        inline void* Calloc(uint32 count, uint32 size)
        {
            size = size + (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));
            ASSERT(mTop + count * size <= mSize, "Buffer is full, cannot allocate memory");

            uint32  offset = mTop + count * size;
            void* ptr = (void*)((int8*)mBuffer + offset);
            mTop = offset;

            return ptr;
        }

        /**
         * Allocate memory for desired size block or crash engine if it does not
         * another free block
         *
         * @param size Size of memory block to be allocated
         * @param alignment Alignment for one block (should be power of 2)
         * @return Pointer to free block
         */
        inline void* Alloc(uint32 size, uint8 alignment)
        {
            size = size + (alignment - (size % alignment));

            ASSERT((alignment - 1) & alignment == 0, "Alignment should be power of 2")
            ASSERT(mTop + size <= mSize, "Buffer is full, cannot allocate memory");

            uint32 offset = mTop + size;
            void* ptr = (void*)((int8*)mBuffer + offset);
            mTop = offset;

            return ptr;
        }

        /**
         * Allocate memory for set of blocks or crash engine if it does not
         * another free block
         *
         * @param count Number of blocks to be allocated
         * @param size Size of one block
         * @param alignment Alignment for one block (should be power of 2)
         * @return Pointer to the first block
         */
        inline void* Calloc(uint32 count, uint32 size, uint8 alignment)
        {
            size = size + (alignment - (size % alignment));

            ASSERT((alignment - 1) & alignment == 0, "Alignment should be power of 2")
            ASSERT(mTop + count * size <= mSize, "Buffer is full, cannot allocate memory");

            uint32  offset = mTop + count * size;
            void* ptr = (void*)((int8*)mBuffer + offset);
            mTop = offset;

            return ptr;
        }

        /**
         * Set marker in current position of the top pointer (allows to free memory
         * to marker position and then allocate from that position)
         */
        inline void SetMarker()
        {
            mMarker = mTop;
        }

        /**
         * Return top pointer to marker position (notice: all the allocated blocks
         * after marker position will be lost)
         */
        inline void FreeToMarker()
        {
            mTop = mMarker;
        }

        /**
         * Return top pointer to start position (0) (notice: all the allocated blocks
         * by buffer will be lost)
         */
        inline void Free()
        {
            mTop = 0;
        }

    private:

        uint32 mSize;
        uint32 mTop;
        uint32 mMarker;
        void* mBuffer;

    };


} // namespace Berserk

#endif //BERSERKENGINE_STACKALLOCATOR_H
