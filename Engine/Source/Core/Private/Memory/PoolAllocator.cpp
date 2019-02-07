//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Public/Misc/Assert.h"
#include "Public/Misc/Alignment.h"
#include "Public/Logging/LogMacros.h"
#include "Public/Memory/Allocator.h"
#include "Public/Memory/PoolAllocator.h"

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
                PUSH("Pool Allocator: free buffer %p", current);

                auto next = current->next;
                Allocator::getSingleton().memoryFree(current);
                current = next;
            }

            mBuffer = nullptr;

            PUSH("Pool Allocator: delete pool");
        }
    }

    void PoolAllocator::Init(uint32 chunkSize, uint32 chunkCount)
    {
        FAIL(chunkSize >= MIN_CHUNK_SIZE, "Chunk size must be more minimum size %u", MIN_CHUNK_SIZE);
        ALIGN(chunkSize);

        mChunkSize = chunkSize;
        mChunkCount = chunkCount;

        mChunk = nullptr;
        mBuffer = nullptr;

        mUsage = 0;
        mTotalSize = 0;

        expand();
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

            while ((uint8*)current + mChunkSize < (uint8*)mBuffer + bufferSize)
            {
                auto next = (Chunk*)((uint8*)current + mChunkSize);
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
                msg, mUsage, mTotalSize, mChunkSize, mChunkCount, sizeof(Buffer) + mChunkCount * mChunkSize);
    }
#endif

}