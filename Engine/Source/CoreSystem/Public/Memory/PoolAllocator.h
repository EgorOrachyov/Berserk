//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_POOLALLOCATOR_H
#define BERSERK_POOLALLOCATOR_H

#include "Misc/Types.h"
#include "Misc/Include.h"
#include "Misc/Buffers.h"
#include "Misc/Compilation.h"
#include "Misc/UsageDescriptors.h"
#include "Object/NewDelete.h"
#include "Memory/IAllocator.h"
#include "Profiling/ProfilingMacro.h"

namespace Berserk
{

    /**
     * @brief Pool Allocation
     *
     * Basic fixed-size blocks pool allocator which gives memory from pre-allocated
     * expandable buffer structured as linked list of free blocks. Allows to get
     * or return block in buffer for const operation time.
     *
     * Pool uses buffers to get another free memory region for allocations. Buffers
     * are structured as linked list, so pool could have a lot of buffers separately
     * stored in the heap
     */
    class MEMORY_API PoolAllocator : public IAllocator
    {
    public:

        /** Cannot create pool for chunks with sizes less than 16 */
        static const uint32 MIN_CHUNK_SIZE = Buffers::SIZE_16;

        /** It is not efficient to create buffers for less than 16 chunks */
        static const uint32 INITIAL_CHUNK_COUNT = Buffers::SIZE_16;

        /** Min chunk count (only for huge chunks of data is actual )*/
        static const uint32 MIN_CHUNK_COUNT = 1;

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

        PoolAllocator() : IAllocator(),
                          mChunkSize(0),
                          mChunkCount(0),
                          mUsage(0),
                          mChunk(nullptr),
                          mBuffer(nullptr),
                          mAllocator(nullptr)
        {

        }

        GENARATE_NEW_DELETE(PoolAllocator);

    public:

        /**
         * Creates pool and initializes that with currently created
         * free blocks of chunkSize size
         * @param chunkSize  Size for one block which could be allocated
         * @param chunkCount Count of chunks in one expand buffer
         */
        PoolAllocator(uint32 chunkSize, uint32 chunkCount, IAllocator* allocator = nullptr);

        ~PoolAllocator() override;

        void nullify()
        {
            mChunkSize = 0;
            mChunkCount = 0;
            mUsage = 0;
            mChunk = nullptr;
            mBuffer = nullptr;
            mAllocator = nullptr;
        }

        /**
         * Takes first free block from list of chunks and returns pointer to that
         * whether list is not empty or expands buffer, marks new free blocks
         * and returns pointer to first currently created block
         *
         * @return Pointer to free chunk
         */
        void *allocate(uint32 size) override;

        /**
         * Try to free chunk with address pointer and return nullptr
         * if succeed
         *
         * @warning Does not check whether the range of the freed pointer belongs to the pool,
         *          therefore to avoid fails with memory allocating you should explicitly
         *          ensure pointer value
         *
         * @return nullptr */
        void free(void *pointer) override;

        /** @return Size of chunk */
        uint32 getChunkSize() const;

        /** @return Chunks count in one buffer */
        uint32 getChunkCount() const;

        /** @return Number of allocated bytes */
        uint32 getUsage() const;

    private:

        /** Creates new buffers and marks free blocks in list if needed */
        void expand();

    #if PROFILE_POOL_ALLOCATOR
        void profile(const char* msg) const;
    #endif

    private:

        uint32  mChunkSize;     // Size of one block for allocation
        uint32  mChunkCount;    // Count of chunks in one buffer
        uint32  mUsage;         // Currently allocated bytes
        Chunk*  mChunk;         // Pointer to the first free chunk
        Buffer* mBuffer;        // Pointer to currently (last) allocated buffer
        IAllocator* mAllocator; // Allocator, used to allocate blocks of memory for pool

    };

}




#endif //BERSERK_POOLALLOCATOR_H
