//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Misc/Assert.h"
#include "Misc/Alignment.h"
#include "Memory/PoolAllocator.h"
#include "Memory/Allocator.h"

namespace Berserk
{

    PoolAllocator::PoolAllocator(uint32 chunkSize, uint32 chunkCount, IAllocator* allocator) : IAllocator()
    {
        FAIL(chunkSize >= MIN_CHUNK_SIZE, "Chunk size must be more minimum size %u", MIN_CHUNK_SIZE);
        FAIL(chunkCount >= MIN_CHUNK_COUNT, "Chunks count must be more than minimum count %u", MIN_CHUNK_COUNT);

        ALIGN(chunkSize);

        mChunkSize = chunkSize;
        mChunkCount = chunkCount;

        mChunk = nullptr;
        mBuffer = nullptr;
        mUsage = 0;

        if (allocator) mAllocator = allocator;
        else mAllocator = &Allocator::getSingleton();

        expand();
    }

    PoolAllocator::~PoolAllocator()
    {
        if (mBuffer != nullptr)
        {
            auto current = mBuffer;
            while (current != nullptr) {

#if PROFILE_POOL_ALLOCATOR
                PUSH("PoolAllocator: free buffer %p", current);
#endif

                auto next = current->next;
                mAllocator->free(current);
                current = next;
            }

            mBuffer = nullptr;

#if PROFILE_POOL_ALLOCATOR
            PUSH("PoolAllocator: delete pool");
#endif
        }
    }

    void * PoolAllocator::allocate(uint32 size)
    {
        expand();

        auto pointer = (void*)mChunk;
        mChunk = mChunk->next;
        mUsage += mChunkSize;

        mAllocCalls += 1;

        return pointer;
    }

    void PoolAllocator::free(void *pointer)
    {
        auto chunk = (Chunk*)(pointer);
        chunk->next = mChunk;
        mChunk = chunk;

        mFreeCalls += 1;

        mUsage -= mChunkSize;
    }

    uint32 PoolAllocator::getChunkSize() const
    {
        return mChunkSize;
    }

    uint32 PoolAllocator::getChunkCount() const
    {
        return mChunkCount;
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
            auto buffer = (Buffer*) mAllocator->allocate(bufferSize);
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
            mTotalMemUsage += mChunkCount * mChunkSize;

        #if PROFILE_POOL_ALLOCATOR
            profile("expand");
        #endif
        }
    }

#if PROFILE_POOL_ALLOCATOR
    void PoolAllocator::profile(const char* msg) const
    {
        PUSH("PoolAllocator: %s: usage: %u | total: %lu | chunk size: %u | chunk count: %u | buffer size: %lu",
                msg, mUsage, mTotalMemUsage, mChunkSize, mChunkCount, sizeof(Buffer) + mChunkCount * mChunkSize);
    }
#endif

}