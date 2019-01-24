//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_POOLALLOCATOR_H
#define BERSERK_POOLALLOCATOR_H

#include "Public/Misc/Types.h"
#include "Public/Misc/Compilation.h"
#include "Public/Misc/UsageDescriptors.h"

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
     */
    class MEMORY_API PoolAllocator {

    public:

        /** Cannot create pool for chunks with sizes less than 16 */
        static const uint32 MIN_CHUNK_SIZE = 16;

        /** It is not efficient to create buffers for less than 16 chunks */
        static const uint32 MIN_CHUNK_COUNT = 16;

    private:

        struct Buffer
        {
            Buffer* next;   // Base offset 0
            uint64  size;   // Base offset 8
                            // Total size 16 (multiple of alignment)
        };

        struct Chunk
        {
            Chunk* next;    // Base offset 0
            uint64 size;    // Base offset 8
                            // Total size 16 (multiple of alignment)
        };

    public:

        /**
         * Creates pool and initializes that with currently created
         * free blocks of chunkSIze size
         * @param chunkSize  Size for one block which could be allocated
         * @param chunkCount Count of chunks in one expand buffer
         */
        PoolAllocator(uint32 chunkSize, uint32 chunkCount);

        ~PoolAllocator();

        /**
         * Takes first free block from list of chunks and returns pointer to that
         * whether list is not empty or expands buffer, marks new free blocks
         * and returns pointer to first currently created block
         *
         * @return Pointer to free chunk
         */
        void* alloc();

        /**
         * Try to free chunk with address pointer and return nullptr
         * if succeed
         *
         * @warning Does not check whether the range of the freed pointer belongs to the pool,
         *          therefore to avoid fails with memory allocating you should explicitly
         *          ensure pointer value
         *
         * @return nullptr */
        void* free(void* pointer);

        /** @return Size of chunk */
        uint32 getChunckSize() const;

        /** @return Chunks count in one buffer */
        uint32 getChunkCount() const;

        /** @return Number of allocated bytes */
        uint32 getUsage() const;

        /** @return Number of potentially allocated bytes */
        uint32 getTotalSize() const;

    private:

        /** Creates new buffers and marks free blocks in list if needed */
        void expand();

    #if DEBUG
        void profile(const char* msg) const;
    #endif

    private:

        uint32  mChunkSize;     // Size of one block for allocation
        uint32  mChunkCount;    // Count of chunks in one buffer
        uint32  mUsage;         // Currently allocated bytes
        uint32  mTotalSize;     // Total size of bytes which could be allocated by pool
        Chunk*  mChunk;         // Pointer to the first free chunk
        Buffer* mBuffer;        // Pointer to currently (last) allocated buffer

    };

}




#endif //BERSERK_POOLALLOCATOR_H
