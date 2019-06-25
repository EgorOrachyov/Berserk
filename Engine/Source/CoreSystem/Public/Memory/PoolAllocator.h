//
// Created by Egor Orachyov on 17.06.2019.
//

#ifndef BERSERK_POOLALLOCATOR_H
#define BERSERK_POOLALLOCATOR_H

#include <Misc/Types.h>
#include <Misc/Include.h>
#include <Object/NewDelete.h>
#include <Memory/Allocator.h>

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

        /** Min size of chunks to create */
        static const uint32 MIN_CHUNK_SIZE = 16;

        /** Default number of chunks to create in single */
        static const uint32 DEFAULT_CHUNK_COUNT = 8;

    public:

        /**
         * Initialize pool and pre-allocate memory
         * @param chunkSize Desired size of chunk for pool
         * @param chunkCount Count of chunks in one pool buffer for expanding
         * @param allocator Allocator to allocate buffers when expand
         */
        PoolAllocator(uint32 chunkSize, uint32 chunkCount = DEFAULT_CHUNK_COUNT, IAllocator& allocator = Allocator::get());

        ~PoolAllocator() override;

        /** @copydoc IAllocator::allocate() */
        void *allocate(uint32 size) override;

        /** @copydoc IAllocator::free() */
        void free(void *pointer) override;

        /** @return Size on pool chunk */
        uint32 getChunkSize() const { return mChunkSize; }

        /** @return Count of chunks in one buffer for expanding */
        uint32 getChunkCount() const { return mChunkCount; }

        /** @return Current memory usage (number of allocated bytes) */
        uint32 getMemoryUsage() const { return mMemoryUsage; }

        /** @return Total free calls for this allocator  */
        uint32 getFreeCalls() const override { return mFreeCalls; }

        /** @return Total allocate calls for this allocator  */
        uint32 getAllocateCalls() const override { return mAllocateCalls; }

        /** @return Total memory usage by this allocator */
        uint64 getTotalMemoryUsage() const override { return mTotalMemoryUsage; }

    protected:

        struct MemoryChunk
        {
        public:

            GENERATE_NEW_DELETE(MemoryChunk);

            /** Links chunk after that */
            void linkAfter(MemoryChunk* next) { mNext = next; }

            /** @return Chunk after that */
            MemoryChunk* next() const { return mNext; }

        private:

            /** Simple linked list of chunks */
            MemoryChunk* mNext = nullptr;

        };

        struct MemoryBuffer
        {
        public:

            GENERATE_NEW_DELETE(MemoryBuffer);

            explicit MemoryBuffer(uint32 size) : mBufferSize(size)
            {

            }

            /**
             * Splits memory buffer into chunks of specified size
             *
             * @note chunkSize * N = mBufferSize, where N - number of chunks to split
             *
             * @param chunkSize Size of one chunk
             * @param buffer Pointer to memory to split
             * @param outHead Pointer to the first chunk
             * @param outTail Pointer to the last chunk
             */
            void split(uint32 chunkSize, void* buffer, MemoryChunk* &outHead, MemoryChunk* &outTail)
            {
                uint32 index = 0;
                uint8* current = (uint8*) buffer;
                uint8* end = current + mBufferSize;

                outHead = new (current) MemoryChunk();
                MemoryChunk* prev = outHead;
                MemoryChunk* chunk = nullptr;

                current += chunkSize;

                while (current < end)
                {
                    chunk = new (current) MemoryChunk();
                    prev->linkAfter(chunk);
                    prev = chunk;

                    current += chunkSize;
                }

                outTail = prev;
            }

            /**
             * Checks, whether chunk belongs to the list of the buffers
             * @param chunk Pointer to chunk to check
             * @return True if chunk belongs to the range of allocated buffers
             */
            bool belongs(MemoryChunk* chunk)
            {
                uint8* buffer = (uint8*)this + sizeof(MemoryBuffer);
                uint8* toCheck = (uint8*) chunk;
                uint8* end = buffer + mBufferSize;

                if (toCheck >= buffer && toCheck < end)
                {
                    return true;
                }
                else
                {
                    if (mNext) return mNext->belongs(chunk);
                    else return false;
                }
            }

            /** Links buffer after that */
            void linkAfter(MemoryBuffer* next) { mNext = next; }

            /** @return Buffer after that */
            MemoryBuffer* next() const { return mNext; }

        private:

            MemoryBuffer* mNext = nullptr;
            uint64 mBufferSize;

        };

    private:

        uint32 mChunkSize = 0;
        uint32 mChunkCount = 0;

        uint32 mAllocateCalls = 0;
        uint32 mFreeCalls = 0;
        uint32 mMemoryUsage = 0;
        uint32 mTotalMemoryUsage = 0;

        MemoryChunk*   mChunks = nullptr;
        MemoryBuffer*  mBuffers = nullptr;
        IAllocator&    mAllocator;

    };

} // namespace Berserk

#endif //BERSERK_POOLALLOCATOR_H
