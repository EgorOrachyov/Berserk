//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_LISTALLOCATOR_H
#define BERSERK_LISTALLOCATOR_H

#include "Public/Misc/Types.h"
#include "Public/Misc/Buffers.h"
#include "Public/Misc/UsageDescriptors.h"

namespace Berserk
{

    /**
     * Standard free list based allocator which gives blocks of suitable size
     * from pre-allocated memory buffer. Allocator could expand its own memory pool
     * to get extra memory to satisfy allocation call
     *
     * Implementation:
     *
     * Buffer 1:
     * _______________________________________________________________________________________________________
     * | size | next (Points |    ______________________________           ______________________________    | -> ... ->
     * |      | to buffer 2) | |- | size | prev | next | empty | -> ... -> | size | prev | next | empty | -> |
     * |      |              |    |______|______|______|_______|           |______|______|______|_______|    |
     * |______|______________|_______________________________________________________________________________|
     *
     * ...
     *
     * Buffer N:
     * _______________________________________________________________________________________________________
     * | size | next (null)  |    ______________________________           ______________________________    |
     * |      |              | -> | size | prev | next | empty | -> ... -> | size | prev | next | empty | -| |
     * |      |              |    |______|______|______|_______|           |______|______|______|_______|    |
     * |______|______________|_______________________________________________________________________________|
     *
     * Buffers blocks creates linked list of buffers
     * Each buffer contains chunks nodes, which contains free blocks of memory
     * Whole chunks from all buffers create doubly linked list of chunks
     *
     * @note Default size of block 24 bytes
     * @note Buffer will expand if it does not find suitable block
     * @warning Use uniteBlocks() when you have enough time for performance
     */
    class MEMORY_API ListAllocator
    {
    public:

        /** Cannot create allocator for buffers less than 1 KiB size (efficiency and usefulness) */
        static const uint32 MIN_BUFFER_SIZE = Buffers::KiB;

        /** One buffer should contain at least 16 chunks of memory */
        static const uint32 MIN_CHUNK_COUNT = Buffers::SIZE_16;

        /** Won't split internal buffers in blocks of size less than 64 bytes */
        static const uint32 MIN_CHUNK_SIZE = Buffers::SIZE_64;

    private:

        struct Buffer
        {
            uint64  size;       // Size of this buffer
            Buffer* next;       // Next buffer in thr buffers list
        };

        struct Chunk
        {
            uint64 size;        // Size of this chunk
            Chunk* prev;        // Previous chunk in thr list
            Chunk* next;        // Next chunk in the list
        };

        struct Block
        {
            uint64 size;        // Size of chunk (we store size of region nearby the allocated memory)
            uint64 data;        // Some 8 data to achieve 16 size of structure (ALIGNMENT == 16)
        };

    public:

        /**
         * Creates and initializes free list allocator
         *
         * @param size  Max size of chunk which could be allocated
         * @param count Max count of that chunks in one internal buffer
         */
        ListAllocator(uint32 size, uint32 count);

        ~ListAllocator();

        /**
         * Allocates block in the buffer of the chosen size
         *
         * @param size Chunk to be allocated
         * @return Pointer to the memory
         */
        void* alloc(uint32 size);

        /**
         * Free allocated chunk of the data
         *
         * @warning The range and the type of pointer will be checked
         *          explicitly by the allocator. Therefore, if you try
         *          to free the pointer not allocated by this allocator,
         *          this allocator will raise an fatal error and shut down engine
         *
         * @param pointer Pointer to the data to be freed
         */
        void free(void* pointer);

        /** @return Max size of allocateble memory block */
        uint32 getChunkSize() const;

        /** @return Count of chunks (of max size) in one internal buffer */
        uint32 getChunkCount() const;

        /** @return Size of one internal buffer */
        uint32 getBufferSize() const;

        /** Currently total allocated memory count [in bytes] */
        uint32 getUsage() const;

        /** Max num of bytes which could be allocated (without expanding) */
        uint32 getTotalSize() const;

    private:

        Buffer* mBuffers;       // First node in the linked list of allocated buffers
        Chunk*  mChunks;        // First chunk which could be returned in alloc() call method
        uint32  mChunkSize;     // Max size of allocatable chunk of memory
        uint32  mChunkCount;    // Count of chunks in one buffer
        uint32  mUsage;         // Currently allocated and used bytes
        uint32  mTotalSize;     // Total size of bytes which could be allocated by allocator

    };

}




#endif //BERSERK_LISTALLOCATOR_H
