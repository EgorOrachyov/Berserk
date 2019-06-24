//
// Created by Egor Orachyov on 17.06.2019.
//

#include "PoolAllocator.h"
#include <Misc/AssertDev.h>
#include <Misc/Alignment.h>

namespace Berserk
{

    PoolAllocator::PoolAllocator(uint32 chunkSize, uint32 chunkCount, IAllocator &allocator)
            : mChunkCount(chunkCount), mAllocator(allocator)
    {
        assert_dev(chunkSize >= MIN_CHUNK_SIZE);
        ALIGN(chunkSize);

        mChunkSize = chunkSize;
        uint32 bufferSize = mChunkCount * mChunkSize;
        uint32 toAllocate = sizeof(MemoryBuffer) + bufferSize;

        uint8* buffer = (uint8*) mAllocator.allocate(toAllocate);
        uint8* memory = buffer + sizeof(MemoryBuffer);

        MemoryChunk* head;
        MemoryChunk* tail;

        mBuffers = new (buffer) MemoryBuffer(bufferSize);
        mBuffers->split(mChunkSize, memory, head, tail);

        mChunks = head;
        mTotalMemoryUsage += toAllocate;
    }

    PoolAllocator::~PoolAllocator()
    {
        if (mBuffers != nullptr)
        {
            assert_dev(mMemoryUsage == 0);

            MemoryBuffer* current = mBuffers;
            while (current != nullptr)
            {
                MemoryBuffer* next = current->next();
                mAllocator.free(current);
                current = next;
            }

            mBuffers = nullptr;
        }
    }

    void *PoolAllocator::allocate(uint32 size)
    {
        assert_dev(size <= mChunkSize);
        if (mChunks == nullptr)
        {
            MemoryChunk* head;
            MemoryChunk* tail;

            uint32 bufferSize = mChunkCount * mChunkSize;
            uint32 toAllocate = sizeof(MemoryBuffer) + bufferSize;

            uint8* buffer = (uint8*) mAllocator.allocate(toAllocate);
            uint8* memory = buffer + sizeof(MemoryBuffer);

            MemoryBuffer* newBuffer = new (buffer) MemoryBuffer(bufferSize);
            newBuffer->linkAfter(mBuffers);

            mBuffers = newBuffer;
            mBuffers->split(mChunkSize, memory, head, tail);

            mChunks = head;
            mTotalMemoryUsage += toAllocate;
        }

        MemoryChunk* chunk = mChunks;
        mChunks = mChunks->next();
        mMemoryUsage += mChunkSize;
        return chunk;
    }

    void PoolAllocator::free(void *pointer)
    {
        MemoryChunk* chunk = (MemoryChunk*) pointer;
        assert_dev(mBuffers->belongs(chunk));
        chunk = new (chunk) MemoryChunk();
        chunk->linkAfter(mChunks);
        mChunks = chunk;
        mMemoryUsage -= mChunkSize;
    }

} // namespace Berserk