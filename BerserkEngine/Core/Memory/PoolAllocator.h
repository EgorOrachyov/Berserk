//
// Created by Egor Orachyov on 26.05.2018.
//

#ifndef BERSERKENGINE_POOLALLOCATOR_H
#define BERSERKENGINE_POOLALLOCATOR_H

#include "../Types.h"
#include "../Common.h"
#include "../Profiling/MemoryProfiler.h"
#include "../MemoryAllocators.h"
#include "../Assert.h"
#include "../UsageDescriptors.h"

namespace Berserk
{

    /**
     * @brief Memory Allocation
     *
     * Basic fixed-size blocks pool allocator which gives memory
     * from pre-allocated buffer structured as linked list of
     * free blocks
     */
    class MEM_API PoolAllocator
    {
    private:

        /**
         * One node block to store free data
         */
        typedef struct Node
        {
        public:
            Node * next;

        } Node;

    public:

        PoolAllocator()
        {
            mElementSize = 0;
            mTotalCount = 0;
            mHead = NULL;
            mBuffer = NULL;
        }

        ~PoolAllocator()
        {
            if (mBuffer)
            { mem_free(mBuffer); }
        }

        /**
         * Set up pool allocator before usage
         *
         * @param elementSize Size of one element (block) to allocate
         * @param maxElementsCount Total number of elements which could be
         *        stored allocated by pool in a single time moment
         */
        inline void Init(uint32 elementSize, uint32 maxElementsCount = 64)
        {
            ASSERT(elementSize >= 8, "Element's size cannot be less than 8 bytes");
            ASSERT(maxElementsCount >= 16, "Element's count cannot be less than 16");

            mElementSize = elementSize + (uint32)(MEMORY_ALIGNMENT - (elementSize % MEMORY_ALIGNMENT));
            mTotalCount = maxElementsCount;

            // Pre-mem_alloc memory for future allocations
            SetUpBuffer();
        }

        /**
         * Free internal buffer and set pool to initial values (0)
         */
        inline void Reset()
        {
            if (mBuffer)
            { mem_free(mBuffer); }

            mElementSize = 0;
            mTotalCount = 0;
            mHead = NULL;
            mBuffer = NULL;
        }

        /**
         * Reset pool and init it with new setting (@see Init @see Reset)
         *
         * @param elementSize Size of one element (block) to allocate
         * @param maxElementsCount Total number of elements which could be
         *        stored allocated by pool in a single time moment
         */
        inline void ReInit(uint32 elementSize, uint32 maxElementsCount = 64)
        {
            Reset();
            Init(elementSize, maxElementsCount);
        }

        /**
         * Get on free block from allocator
         *
         * @return Pointer to free block
         */
        inline void* AllocBlock()
        {
            ASSERT(mHead, "Head is NULL, cannot allocate block");

            void* ptr = (void*)mHead;
            mHead = mHead->next;

            return ptr;
        }

        /**
         * Mark block as free and return to pool
         *
         * @param block
         */
        inline void FreeBlock(void* block)
        {
            ASSERT(block >= mBuffer, "Block out of buffer range");
            ASSERT(block < (int8*)mBuffer + (mElementSize * mTotalCount), "Block out of buffer range");

            Node* node = mHead;
            mHead = (Node*)block;
            mHead->next = node;
        }

    private:

        /**
         *  Allocates memory for internal buffer and marks
         *  list of free blocks
         */
        inline void SetUpBuffer()
        {
            ASSERT(!mBuffer, "Buffer should be NULL before init");

            mBuffer = mem_calloc(mTotalCount, mElementSize);
            mHead = (Node*)mBuffer;

            Node* current = mHead;
            for(uint32 i = 0; i < mTotalCount - 1; i++)
            {
                Node* next = (Node*)((int8*)current + mElementSize);
                current->next = next;
                current = next;
            }

            current->next = NULL;
        }

        /**
         * Debug function
         */
        inline void PrintInfo()
        {
            printf("Pool Allocator\nElement Size:%i\nTotal Count:%i\n", mElementSize, mTotalCount);

            int32 i = 0;
            Node* current = mHead;
            while (current)
            {
                printf("[%3i] curr: %p next: %p\n", i++, current, current->next);
                current = current->next;
            }
        }

    private:

        uint32 mElementSize;
        uint32 mTotalCount;
        Node* mHead;
        void* mBuffer;

    };

} // namespace Berserk

#endif //BERSERKENGINE_POOLALLOCATOR_H
