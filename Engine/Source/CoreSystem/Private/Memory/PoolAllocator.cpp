//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Misc/Assert.h"
#include "Misc/Alignment.h"
#include "Logging/LogMacros.h"
#include "Memory/PoolAllocator.h"
#include "Memory/Allocator.h"

namespace Berserk
{

    PoolAllocator::PoolAllocator(uint32 chunkSize, uint32 chunkCount)
    {
        FAIL(chunkSize >= MIN_CHUNK_SIZE, "Chunk size must be more minimum size %u", MIN_CHUNK_SIZE);
        FAIL(chunkCount >= MIN_CHUNK_COUNT, "Chunks count must be more than minimum count %u", MIN_CHUNK_COUNT);

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
        if (mBuffer != nullptr)
        {
            auto current = mBuffer;
            while (current != nullptr) {
                PUSH("PoolAllocator: free buffer %p", current);

                auto next = current->next;
                Allocator::getSingleton().free(current);
                current = next;
            }

            mBuffer = nullptr;

            PUSH("PoolAllocator: delete pool");
        }
    }

    void* PoolAllocator::alloc()
    {
        expand();

        auto pointer = (void*)mChunk;
        mChunk = mChunk->next;
        mUsage += mChunkSize;

        return pointer;
    }

    void* PoolAllocator::free(void *pointer)
    {
        auto chunk = (Chunk*)(pointer);
        chunk->next = mChunk;
        mChunk = chunk;

        mUsage -= mChunkSize;

        return nullptr;
    }

    uint32 PoolAllocator::getChunkSize() const
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
            auto bufferSize = sizeof(Buffer) + mChunkSize * mChunkCount;
            auto buffer = (Buffer*) Allocator::getSingleton().allocate(bufferSize);
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

            while ((uint8*)current + mChunkSize < (uint8*)mBuffer + bufferSize)
            {
                auto next = (Chunk*)((uint8*)current + mChunkSize);
                current->next = next;
                current = next;
            }

            current->next = nullptr;
            mTotalSize += mChunkCount * mChunkSize;

        #if PROFILE_POOL_ALLOCATOR
            profile("expand");
        #endif
        }
    }

#if PROFILE_POOL_ALLOCATOR
    void PoolAllocator::profile(const char* msg) const
    {
        PUSH("PoolAllocator: %s: usage: %u | total: %u | chunk size: %u | chunk count: %u | buffer size: %lu",
                msg, mUsage, mTotalSize, mChunkSize, mChunkCount, sizeof(Buffer) + mChunkCount * mChunkSize);
    }
#endif

}