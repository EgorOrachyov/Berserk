//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_DOUBLESTACKALLOCATOR_H
#define BERSERKENGINE_DOUBLESTACKALLOCATOR_H

#include "Essential/Types.h"
#include "Essential/UsageDescriptors.h"

namespace Berserk
{

    /**
     * @brief Memory Allocation
     *
     * Basic double stack allocator which gives memory from pre-allocated fixed size
     * buffer. Allows set 2 markers and free buffer for const operation time.
     * Does not allow random free of allocated memory.
     *
     * Allows to use head and tail buffer parts for proper allocation of long term and short
     * term memory data.
     */
    class MEM_API DoubleStackAllocator
    {
    public:


        DoubleStackAllocator();
        ~DoubleStackAllocator();

        /**
         * Init stack and pre-allocate buffer
         *
         * @param size Size of stack buffer in bytes (cannot be less than 64)
         */
        void init(UINT32 size = 64);

        /**
         * Free internal buffer and set stack to initial values (0) (notice: all the
         * allocated variables by this stack will be lost)
         */
        void destroy();

        /**
         * Allocate memory in the head part for desired size block or crash engine if it does not have
         * another free block (uses standard alignment MEMORY_ALIGNMENT)
         *
         * @param size Size of memory block to be allocated
         * @return Pointer to free block
         */
        void* allocHead(UINT32 size);

        /**
         * Allocate memory in the tail part for desired size block or crash engine if it does not have
         * another free block (uses standard alignment MEMORY_ALIGNMENT)
         *
         * @param size Size of memory block to be allocated
         * @return Pointer to free block
         */
        void* allocTail(UINT32 size);

        /**
         * Allocate memory in the head part for set of blocks or crash engine if it does not have
         * another free block (uses standard alignment MEMORY_ALIGNMENT)
         *
         * @param count Number of blocks to be allocated
         * @param size Size of one block
         * @return Pointer to the first block
         */
        void* callocHead(UINT32 count, UINT32 size);

        /**
         * Allocate memory in the tail part for set of blocks or crash engine if it does not have
         * another free block (uses standard alignment MEMORY_ALIGNMENT)
         *
         * @param count Number of blocks to be allocated
         * @param size Size of one block
         * @return Pointer to the first block
         */
        void* callocTail(UINT32 count, UINT32 size);

        /**
         * Free head part of stack (all the allocated data by *allocHead will be lost)
         */
        void resetHead();

        /**
         * Free tail part of stack (all the allocated data by *allocTail will be lost)
         */
        void resetTail();

        /**
         * Set marker in current head position of the top pointer (allows to free memory
         * to marker position and then allocate from that position)
         */
        void setMarkerHead();

        /**
         * Set marker in current tail position of the top pointer (allows to free memory
         * to marker position and then allocate from that position)
         */
        void setMarkerTail();

        /**
         * Free buffer (all the allocated data will be lost)
         *
         * @note equal to call of resetHead() and resetTail()
         */
        void free();

        /**
         * Return head pointer to head marker position (notice: all the allocated blocks
         * after marker position will be lost)
         */
        void freeToHeadMarker();

        /**
         * Return tail pointer to tail marker position (notice: all the allocated blocks
         * after marker position will be lost)
         */
        void freeToTailMarker();

    private:

        UINT32 mHead;
        UINT32 mTail;
        UINT32 mHeadMarker;
        UINT32 mTailMarker;
        UINT32 mSize;
        void* mBuffer;

    };

} // namespace Berserk

#endif //BERSERKENGINE_DOUBLESTACKALLOCATOR_H