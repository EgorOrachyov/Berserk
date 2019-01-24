//
// Created by Egor Orachyov on 24.01.2019.
//

#include <string>
#include "Public/Misc/Assert.h"
#include "Public/Misc/Alignment.h"
#include "Public/Memory/Allocator.h"
#include "Public/Memory/PoolAllocator.h"

namespace Berserk
{

    PoolAllocator::PoolAllocator(uint32 chunkSize, uint32 chunkCount)
    {
        ASSERT(chunkSize >= MINIM_CHUNK_SIZE, "Chunk size must be more minimum size %ui", MINIM_CHUNK_SIZE);
        ASSERT(chunkCount >= MINIM_CHUNK_COUNT, "Chunks count must be more than minimum count %ui", MINIM_CHUNK_COUNT);

        ALIGN(chunkSize);

        mChunkSize = chunkSize;
        mChunkCount = chunkCount;

        mChunk = nullptr;
        mBuffer = nullptr;

        mUsage = 0;
        mTotalSize = 0;

        expand();
    }

    PoolAllocator::~PoolAllocator()
    {
        if (mBuffer == nullptr)
        {
            // warning
        }
        else
        {
            auto current = mBuffer;
            while (current != nullptr) {
                fprintf(stdout, "Pool Allocator: free buffer %p\n", current);

                auto next = current->next;
                Allocator::getSingleton().memoryFree(current);
                current = next;
            }
        }

        fprintf(stdout, "Pool Allocator: delete pool");
    }

    void* PoolAllocator::alloc()
    {
        expand();

        void* pointer = (uint8*)mChunk + sizeof(Chunk);
        mChunk = mChunk->next;
        mUsage += mChunkSize;

        return pointer;
    }

    void* PoolAllocator::free(void *pointer)
    {
        auto chunk = (Chunk*)((uint8*)pointer - sizeof(Chunk));
        chunk->next = mChunk;
        mChunk = chunk;
    }

    uint32 PoolAllocator::getChunckSize() const
    {
        return mChunkSize;
    }

    uint32 PoolAllocator::getChunkCount() const
    {
        return mChunkCount;
    }

    uint32 PoolAllocator::getTotalSize() const
    {
        return mTotalSize;
    }

    uint32 PoolAllocator::getUsage() const
    {
        return mUsage;
    }

    void PoolAllocator::expand()
    {
        if (mChunk == nullptr)
        {
            auto chunkSize = sizeof(Chunk) + mChunkSize;
            auto bufferSize = sizeof(Buffer) + chunkSize * mChunkCount;
            auto buffer = (Buffer*) Allocator::getSingleton().memoryAllocate(bufferSize);
            buffer->size = bufferSize;

            if (mBuffer)
            {
                buffer->next = mBuffer;
                mBuffer = buffer;
            }
            else
            {
                mBuffer = buffer;
                buffer->next = nullptr;
            }

            auto current = (Chunk*)((uint8*)mBuffer + sizeof(Buffer));
            mChunk = current;

            while ((uint8*)current + chunkSize < (uint8*)mBuffer + bufferSize)
            {
                // current->size = mChunkSize; not used
                auto next = (Chunk*)((uint8*)current + chunkSize);
                current->next = next;
                current = next;
            }

            current->next = nullptr;
            mTotalSize += mChunkCount * mChunkSize;

        #if DEBUG
            profile("expand");
        #endif
        }
    }

#if DEBUG
    void PoolAllocator::profile(const char* msg) const
    {
        fprintf(stdout,
                "Pool Allocator: %s: usage: %u | total: %u | chunk size: %u | chunk count: %u | buffer size: %lu\n",
                msg, mUsage, mTotalSize, mChunkSize, mChunkCount, sizeof(Buffer) + mChunkCount * (sizeof(Chunk) + mChunkSize));
    }
#endif

}