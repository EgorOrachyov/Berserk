//
// Created by Egor Orachyov on 26.05.2018.
//

#ifndef BERSERKENGINE_POOLALLOCATOR_H
#define BERSERKENGINE_POOLALLOCATOR_H

#include "../Essential/Types.h"
#include "../Essential/UsageDescriptors.h"

namespace Berserk
{

    /**
     * @brief Memory Allocation
     *
     * Basic fixed-size blocks pool allocator which gives memory from pre-allocated
     * expandable buffer structured as linked list of free blocks. Allows to get
     * or return block in buffer for const operation time.
     *
     * Pool uses buffers to get another free memory region for allocations. Buffers
     * are structured as linked list, so pool could have a lot of buffers separately
     * stored in the heap
     *
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
            Node* next;
        } Node;

        /**
         * Buffers of memory
         */
        typedef struct Buffer
        {
        public:
            Buffer* next;
        } Buffer;

    public:

        /**
         * Initialize pool with 0 values
         */
        PoolAllocator();

        /**
         * Destroy pool and free buffers (if it is allocated)
         */
        ~PoolAllocator();

        /**
         * Set up pool allocator before usage
         * (uses standard alignment MEMORY_ALIGNMENT for one element)
         *
         * @warning Elements count for one buffer should be more than 16
         *
         * @param elementSize Size of one element (block) to allocate
         * @param elementsCountInOneBuffer Total number of elements which could be
         *        stored in one buffer
         */
        void init(uint16 elementSize, uint16 elementsCountInOneBuffer);

        /**
         * Free internal buffers and set pool to initial values (0) (notice: all the
         * allocated blocks by this pool will be lost)
         */
        void reset();

        /**
         * Reset pool and init it with new setting (@see Init @see Reset)
         *
         * @warning Elements count for one buffer should be more than 16
         *
         * @param elementSize Size of one element (block) to allocate
         * @param elementsCountInOneBuffer Total number of elements which could be
         *        stored in one buffer
         */
        void reInit(uint16 elementSize, uint16 elementsCountInOneBuffer);

        /**
         * Get one free block from allocator if it has another one if it is
         * not engine will crash
         *
         * @return Pointer to free block
         */
        void* allocBlock();

        /**
         * Mark block as free and return to pool
         *
         * @param block
         */
        void freeBlock(void *block);

        /**
         * Get max num of block which can be allocated by this pool
         * without expanding
         *
         * @return Max blocks' count
         */
        uint16 getCapacity() const;

        /**
         * Get number of free blocks which could be allocated
         *
         * @return Number of available free blocks
         */
        uint16 getNumOfFreeBlocks() const;

        /**
         * Get element size in bytes
         *
         * @return Bytes of size
         */
        uint32 getElementSize() const;

        /**
         * Get max number of elements for one buffer
         *
         * @return One buffer max number of elements
         */
        uint32 getOneBufferCapacity() const;

        /**
         * Return true if expanding is locked
         *
         * @return
         */
        bool isStatic() const;

        /**
         * Lock the ability to expand internal memory byffers of the pool
         *
         * @param lookExpanding (true to lock)
         */
        void setStatic(bool lookExpanding);

        /**
         * Allows to set one buffer max number of elements
         *
         * @warning Elements count for one buffer should be more than 16
         *
         * @param capacity Desired number of elements
         */
        void setOneBufferCapacity(uint32 capacity);

    public:

        /**
         * Allocate memory for internal buffer and marks list of free blocks
         */
        void setUpBuffer();

        /**
         * Allocates another buffer and marks nodes to list
         * @note Works if isStatic == false
         */
        void expandBuffer();

        /**
         * Debug printing function for output console
         */
        void printInfo();

    private:

        uint32 mElementSize;            // Size in bytes of 1 element
        uint32 mOneBufferCount;         // Capacity in elements of one buffer
        uint16 mTotalCount;             // Max count of elements
        uint16 mCurrentCount;           // Current count of free blocks
        uint16 mIsStatic;               // Lock the expand ability
        Node* mHead;                    // Last free block
        Buffer* mBuffer;                // Last memory buffer

    };

} // namespace Berserk

#endif //BERSERKENGINE_POOLALLOCATOR_H
