//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_DOUBLEFRAMEMANAGER_H
#define BERSERKENGINE_DOUBLEFRAMEMANAGER_H

#include "../Essential/Types.h"
#include "../Essential/UsageDescriptors.h"

#include "StackAllocator.h"

namespace Berserk
{

    /**
     * Allocator are based on two stack allocators of the same size.
     * Works as simple stack allocator
     *
     * @note It allows you to allocate memory in a current frame and the use
     * this memory in the next frame.
     *
     * @warning Before each frame you should call swap() to change the role of
     * the current and previous allocators.
     */
    class MEMORY_API DoubleFrameAllocator
    {
    public:

        DoubleFrameAllocator();
        ~DoubleFrameAllocator();

        /**
         * Init stack and pre-allocate buffer
         *
         * @param size Size of stack buffer in bytes (cannot be less than 64)
         */
        void init(UINT32 size = 64);

        /**
         * Swap internal stack allocators to work with another in current frame stage
         */
        void swap();

        /**
         * Free internal buffer and set stack to initial values (0) (notice: all the
         * allocated variables by this stack will be lost)
         */
        void reset();

        /**
         * Reset stack and init it with new setting (@see Init @see Reset)
         *
         * @param size Size of stack buffer in bytes (cannot be less than 64)
         */
        void reInit(UINT32 size = 64);

        /**
         * Allocate memory for desired size block or crash engine if it does not have
         * another free block (uses standard alignment MEMORY_ALIGNMENT)
         *
         * @param size Size of memory block to be allocated
         * @return Pointer to free block
         */
        void* allocBlock(UINT32 size);

        /**
         * Allocate memory for set of blocks or crash engine if it does not have
         * another free block (uses standard alignment MEMORY_ALIGNMENT)
         *
         * @param count Number of blocks to be allocated
         * @param size Size of one block
         * @return Pointer to the first block
         */
        void* callocBlock(UINT32 count, UINT32 size);

        /**
         * Allocate memory for desired size block or crash engine if it does not have
         * another free block
         *
         * @param size Size of memory block to be allocated
         * @param alignment Alignment for one block (should be power of 2)
         * @return Pointer to free block
         */
        void* allocBlock(UINT32 size, UINT8 alignment);

        /**
         * Allocate memory for set of blocks or crash engine if it does not have
         * another free block
         *
         * @param count Number of blocks to be allocated
         * @param size Size of one block
         * @param alignment Alignment for one block (should be power of 2)
         * @return Pointer to the first block
         */
        void* callocBlock(UINT32 count, UINT32 size, UINT8 alignment);

        /**
         * Return top pointer to start position (0) (notice: all the allocated blocks
         * by buffer will be lost)
         */
        void free();

    private:

        StackAllocator mFirstAlloc;
        StackAllocator mSecondAlloc;
        StackAllocator* mCurrent;
        StackAllocator* mPrevious;

    };
}

#endif //BERSERKENGINE_DOUBLEFRAMEMANAGER_H