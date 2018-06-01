//
// Created by Egor Orachyov on 26.05.2018.
//

#ifndef BERSERKENGINE_POOLALLOCATOR_H
#define BERSERKENGINE_POOLALLOCATOR_H

#include "../Types.h"
#include "../UsageDescriptors.h"

namespace Berserk
{

    /**
     * @brief Memory Allocation
     *
     * Basic fixed-size blocks pool allocator which gives memory from pre-allocated
     * fixed-size buffer structured as linked list of free blocks. Allows to get
     * or return block in buffer for const operation time.
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

        PoolAllocator();
        ~PoolAllocator();

        /**
         * Set up pool allocator before usage
         * (uses standard alignment MEMORY_ALIGNMENT for one element)
         *
         * @param elementSize Size of one element (block) to allocate
         * @param maxElementsCount Total number of elements which could be
         *        stored allocated by pool in a single time moment
         */
        void Init(uint32 elementSize, uint32 maxElementsCount = 64);

        /**
         * Free internal buffer and set pool to initial values (0) (notice: all the
         * allocated blocks by this pool will be lost)
         */
        void Reset();

        /**
         * Reset pool and init it with new setting (@see Init @see Reset)
         *
         * @param elementSize Size of one element (block) to allocate
         * @param maxElementsCount Total number of elements which could be
         *        stored allocated by pool in a single time moment
         */
        void ReInit(uint32 elementSize, uint32 maxElementsCount = 64);

        /**
         * Get one free block from allocator if it has another one if it is
         * not engine will crash
         *
         * @return Pointer to free block
         */
        void* AllocBlock();

        /**
         * Mark block as free and return to pool
         *
         * @param block
         */
        void FreeBlock(void* block);

    private:

        /**
         *  Allocate memory for internal buffer and marks list of free blocks
         */
        void SetUpBuffer();

        /**
         * Debug printing function for output console
         */
        void PrintInfo();

    private:

        uint32 mElementSize;
        uint32 mTotalCount;
        Node* mHead;
        void* mBuffer;

    };

} // namespace Berserk

#endif //BERSERKENGINE_POOLALLOCATOR_H
