//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_LISTALLOCATOR_H
#define BERSERK_LISTALLOCATOR_H

#include "Misc/Types.h"
#include "Misc/Buffers.h"
#include "Misc/Compilation.h"
#include "Misc/UsageDescriptors.h"
#include "Memory/IAllocator.h"
#include "Memory/Allocator.h"
#include "Profiling/ProfilingMacro.h"

namespace Berserk
{

    /**
     * Standard free list based allocator which gives blocks of suitable size
     * from pre-allocated memory buffer. Allocator could expand its own memory pool
     * to get extra memory to satisfy allocation call.
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
     */
    class MEMORY_API ListAllocator : public IAllocator
    {
    public:

        /** Cannot create allocator for buffers less than 1 KiB size (efficiency and usefulness) */
        static const uint32 MIN_BUFFER_SIZE = Buffers::KiB;

        /** Won't split internal buffers in blocks of size less than 64 bytes */
        static const uint32 MIN_SPLIT_SIZE = Buffers::SIZE_64;

    private:

        struct Buffer
        {
            uint64  size;       // Size of buffer
            Buffer* next;       // Pointer to the next buffer in the list
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
         * @param bufferSize Max size of chunk which could be allocated
         *                   should more then MIN_CHUNK_SIZE
         * @param allocator  Allocator, which allocates memory for this one
         */
        explicit ListAllocator(uint32 bufferSize, IAllocator* allocator = nullptr);

        ~ListAllocator() override;

        /**
         * Allocates block in the buffer of the chosen size
         *
         * @param size Chunk to be allocated
         * @return Pointer to the memory
         */
        void* allocate(uint32 size) override;

        /**
         * Free allocated chunk of the data
         *
         * @warning The range and the type of pointer must be checked
         *          explicitly by the user. Therefore, if you try
         *          to free the pointer not allocated by this allocator,
         *          this will cause an fatal memory error
         *
         * @param pointer Pointer to the data to be freed
         */
        void free(void* pointer) override;

        /** @return Max size of allocateble memory block */
        uint32 getChunkSize() const;

        /** @return Size of one internal buffer */
        uint32 getBufferSize() const;

        /** Currently total allocated memory count [in bytes] */
        uint32 getUsage() const;

    private:

        /**
         * Try to split source chunk on two 2 parts to provide left
         * chunk of size size and the rest of the source chunk in the right
         * @param source        Chunk to split
         * @param size          Size of left chunk to get
         * @param[out] left     Chunk of size size
         * @param[out] right    The rest of the source chunk
         */
        void split(Chunk* source, uint32 size, Chunk** left, Chunk** right);

        /** Join free chunks in one big chunk */
        Chunk* join(Chunk* left, Chunk* right);
        Chunk* join(Chunk* left, Chunk* mid, Chunk* right);

        /** Checks whether chunks are neighbors */
        bool couldJoin(Chunk* left, Chunk* right);
        bool couldJoin(Chunk* left, Chunk* mid, Chunk* right);

        /** Create or expand internal allocator memory pool */
        void expand();

    public:

        void profile(const char* msg);
        void blocks(const char* msg);

    private:

        IAllocator * mAllocator;    // Allocator, which allocates memory for this one
        Buffer* mBuffer;            // First buffer in the list of buffers
        Chunk*  mChunk;             // First chunk which could be returned in allocate() call method
        uint32  mBufferSize;        // Max size of allocatable chunk of memory
        uint32  mUsage;             // Currently allocated and used bytes

    };

}




#endif //BERSERK_LISTALLOCATOR_H
