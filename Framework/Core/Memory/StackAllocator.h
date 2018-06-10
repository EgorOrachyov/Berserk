//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_STACKALLOCATOR_H
#define BERSERKENGINE_STACKALLOCATOR_H

#include "../Essential/Types.h"
#include "../Essential/UsageDescriptors.h"

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

        StackAllocator();
        ~StackAllocator();

        /**
         * Init stack and pre-allocate buffer
         *
         * @param size Size of stack buffer in bytes (cannot be less than 64)
         */
        void Init(uint32 size = 64);

        /**
         * Free internal buffer and set stack to initial values (0) (notice: all the
         * allocated variables by this stack will be lost)
         */
        void Reset();

        /**
         * Reset stack and init it with new setting (@see Init @see Reset)
         *
         * @param size Size of stack buffer in bytes (cannot be less than 64)
         */
        void ReInit(uint32 size = 64);

        /**
         * Allocate memory for desired size block or crash engine if it does not
         * another free block (uses standard alignment MEMORY_ALIGNMENT)
         *
         * @param size Size of memory block to be allocated
         * @return Pointer to free block
         */
        void* Alloc(uint32 size);

        /**
         * Allocate memory for set of blocks or crash engine if it does not
         * another free block (uses standard alignment MEMORY_ALIGNMENT)
         *
         * @param count Number of blocks to be allocated
         * @param size Size of one block
         * @return Pointer to the first block
         */
        void* Calloc(uint32 count, uint32 size);

        /**
         * Allocate memory for desired size block or crash engine if it does not
         * another free block
         *
         * @param size Size of memory block to be allocated
         * @param alignment Alignment for one block (should be power of 2)
         * @return Pointer to free block
         */
        void* Alloc(uint32 size, uint8 alignment);

        /**
         * Allocate memory for set of blocks or crash engine if it does not
         * another free block
         *
         * @param count Number of blocks to be allocated
         * @param size Size of one block
         * @param alignment Alignment for one block (should be power of 2)
         * @return Pointer to the first block
         */
        void* Calloc(uint32 count, uint32 size, uint8 alignment);

        /**
         * Set marker in current position of the top pointer (allows to free memory
         * to marker position and then allocate from that position)
         */
        void SetMarker();

        /**
         * Return top pointer to marker position (notice: all the allocated blocks
         * after marker position will be lost)
         */
        void FreeToMarker();

        /**
         * Return top pointer to start position (0) (notice: all the allocated blocks
         * by buffer will be lost)
         */
        void Free();

    private:

        uint32 mSize;
        uint32 mTop;
        uint32 mMarker;
        void* mBuffer;

    };


} // namespace Berserk

#endif //BERSERKENGINE_STACKALLOCATOR_H
