//
// Created by Egor Orachyov on 09.08.2018.
//

#ifndef BERSERKENGINE_LISTALLOCATOR_H
#define BERSERKENGINE_LISTALLOCATOR_H

#include "Essential/Types.h"
#include "Essential/Assert.h"
#include "Essential/UsageDescriptors.h"
#include "Math/UtilityNumbers.h"
#include "Memory/MemoryAllocators.h"

namespace Berserk
{

    /**
     * Standard free list based allocator which gives blocks of suitable size
     * from pre-allocated memory buffer, returns blocks in the list and calls
     * destructors for special Element types.
     *
     * @note Default size of block 24 bytes
     * @note Buffer will expand if it does not find suitable block
     * @warning Use uniteBlocks() when you have enough time for performance
     */
    class MEMORY_API ListAllocator
    {
    private:

        /**
         * List allocator block, minimal size is 24 bytes (!!!)
         */
        struct Block
        {
        public:
            Block* next;
            Block* prev;
            UINT64 size;
        };

        struct Buffer
        {
        public:
            Buffer* next;
            Block data;
        };

    public:

        ListAllocator();
        ~ListAllocator();

        /**
         * Initialize list allocator with desired buffer size
         * @note Buffer won't expand therefore if list full it won't alloc memory
         * @param bufferSize Internal buffer size (def value is 1024 bytes)
         */
        void init(UINT32 bufferSize = BUFFER_SIZE_1024);

        /**
         * Reset buffer an free used memory
         */
        void reset();

        /**
         * Alloc block of desired size
         * @note Uses standard alignment
         * @param size Size of needed block
         * @return Pointer to data
         */
        ENGINE_DEPRECATED void* alloc(UINT32 size);

        /**
         * Alloc block of desired size and alignment
         * @param size Size of needed block
         * @param alignment Should be power of 2
         * @return Pointer to data
         */
        ENGINE_DEPRECATED void* alloc(UINT32 size, UINT32 alignment);

        /**
         * Alloc mem for type element
         * @tparam Element type of allocating data
         * @return Pointer to element's data
         */
        template <typename Element>
        Element* alloc();

        /**
         * Alloc mem for type element and init it via copy of data
         * @tparam Element type of allocating data
         * @param data Data to copy in the allocated block
         * @return Pointer to element's data
         */
        template <typename Element>
        Element* allocCopy(const Element* data);

        /**
         * Return block to the list
         * @warning Block should be allocated by THAT list
         * @param block Pointer to data
         * @return NULL if successfully free
         */
        ENGINE_DEPRECATED void* free(void* block, UINT32 size);

        /**
         * Return block to the list (if use custom alignment)
         * @warning Block should be allocated by THAT list
         * @param block Pointer to data
         * @param alignment Used alignment for allocation
         * @return NULL if successfully free
         */
        ENGINE_DEPRECATED void* free(void* block, UINT32 size, UINT32 alignment);

        /**
         * Return block to the list and call default DESTRUCTOR for the element
         * @tparam Element Type of element
         * @param element Pointer to data
         * @return NULL if successfully free
         */
        template <typename Element>
        void* free(Element* element);

        /**
         * Returns free blocks in the list and unites its in the blocks
         * of another suitable size
         * @note It can take a while
         */
        void uniteBlocks();

        /**
         * Get pointer to buffer
         * @return Pointer
         */
        void*  getBuffer() const;

        /**
         * Get size of buffer for expanding
         * @return Bytes
         */
        UINT32 getExpandBufferSize() const;

        /**
         * Number of allocated buffers by this list
         * @return Count
         */
        UINT32 getNumOfBuffers() const;

        /**
         * Get size of buffer
         * @return Size
         */
        UINT32 getBufferSize() const;

        /**
         * Total memory got from os by list to alloc
         * @return Bytes
         */
        UINT32 getMemoryUsage() const;

        /**
         * Max num of bytes which can be allocated by list
         * @return Bytes
         */
        UINT32 getMaxAvailableMemory() const;

        /**
         * Get bytes of allocated memory
         * @return Bytes
         */
        UINT32 getAllocatedMemory() const;

        /**
         * Get minimal size of one internal block
         * @return Size
         */
        UINT32 getMinimalBlockSize() const;

        DEBUG_API void printBlocksInfo() const;
        DEBUG_API void printFreeBlockInfo() const;
        DEBUG_API void printBuffersInfo() const;

    private:

        void expand();
        void returnBlock(void* block, UINT32 size);
        void createList();
        void* getBlock(UINT32 size);

    private:

        UINT32 mNumOfBuffers;
        UINT32 mExpandSize;
        UINT32 mBufferSize;
        UINT32 mAllocatedMemory;

        Buffer* mBuffer;
        Block* mFirst;
        Block* mListOfFreeBlocks;

    };

} // namespace Berserk

#include "Source/ListAllocator.cpp"

#endif //BERSERKENGINE_LISTALLOCATOR_H