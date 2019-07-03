//
// Created by Egor Orachyov on 17.06.2019.
//

#include "Memory/PoolAllocator.h"
#include <Misc/AssertDev.h>
#include <Misc/Alignment.h>

namespace Berserk
{

    PoolAllocator::PoolAllocator(uint32 chunkSize, uint32 chunkCount, IAllocator &allocator)
            : mChunkCount(chunkCount), mAllocator(allocator)
    {
        assertion_dev(chunkSize >= MIN_CHUNK_SIZE);
        ALIGNMENT(chunkSize);
        mChunkSize = chunkSize;
    }

    PoolAllocator::~PoolAllocator()
    {
        if (mBuffers != nullptr)
        {
            char buffer[20];
            assertion_dev_msg(mAllocateCalls == mFreeCalls,
                              "Allocator(Global): [allocation calls: %u] [free calls: %u] [total memory usage: %s]\n",
                              mAllocateCalls, mFreeCalls, Printer::print((uint32)getTotalMemoryUsage(), buffer));

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

    void* PoolAllocator::allocate(uint32 size)
    {
        return PoolAllocator::allocate(size, MEMORY_ALIGNMENT);
    }

    void *PoolAllocator::allocate(uint32 size, uint32 alignment)
    {
        assertion_dev(size <= mChunkSize);
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
        mAllocateCalls += 1;
        return chunk;
    }

    void PoolAllocator::free(void *pointer)
    {
        MemoryChunk* chunk = (MemoryChunk*) pointer;
        assertion_dev(mBuffers->belongs(chunk));
        chunk = new (chunk) MemoryChunk();
        chunk->linkAfter(mChunks);
        mChunks = chunk;
        mMemoryUsage -= mChunkSize;
        mFreeCalls += 1;
    }

} // namespace Berserk