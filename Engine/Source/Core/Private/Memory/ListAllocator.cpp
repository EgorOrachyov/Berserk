//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Public/Misc/Assert.h"
#include "Public/Misc/Alignment.h"
#include "Public/Math/MathUtility.h"
#include "Public/Memory/ListAllocator.h"

namespace Berserk
{

    ListAllocator::ListAllocator(uint32 chunkSize)
            : mPool(Math::max(chunkSize, MIN_CHUNK_SIZE) * MIN_CHUNK_COUNT + sizeof(Buffer), MIN_BUFFERS_COUNT)
    {
        ASSERT(chunkSize >= MIN_CHUNK_SIZE , "Chunk size must be more than %u", MIN_CHUNK_SIZE);
        ALIGN(chunkSize);

        mUsage = 0;
        mChunk = nullptr;
        mBuffer = nullptr;
        mChunkSize = chunkSize;

        expand();
    }

    ListAllocator::~ListAllocator()
    {

    }

    void* ListAllocator::alloc(uint32 size)
    {

    }

    void ListAllocator::free(void *pointer)
    {

    }

    uint32 ListAllocator::getChunkSize() const
    {
        return mChunkSize;
    }

    uint32 ListAllocator::getChunkCount() const
    {
        return MIN_CHUNK_COUNT;
    }

    uint32 ListAllocator::getBufferSize() const
    {
        return MIN_CHUNK_COUNT * mChunkSize;
    }

    uint32 ListAllocator::getUsage() const
    {
        return mUsage;
    }

    uint32 ListAllocator::getTotalSize() const
    {
        return mPool.getTotalSize();
    }

    void ListAllocator::expand()
    {
        auto buffer = (Buffer*) mPool.alloc();
        buffer->next = mBuffer;
        buffer->size = mChunkSize * MIN_CHUNK_COUNT;
        mBuffer = buffer;

        auto current = (Chunk*)((uint8*)mBuffer + sizeof(Buffer));
        auto first = current;
        current->prev = nullptr;
        current->size = mChunkSize;

        while ((uint8*)current + mChunkSize < (uint8*)mBuffer + sizeof(Buffer) + mBuffer->size)
        {
            auto next = (Chunk*)((uint8*)current + mChunkSize);

            next->prev = current;
            next->size = mChunkSize;

            current->next = next;
            current = next;
        }

        if (mChunk)
        {
            current->next = mChunk;
            mChunk->prev = current;
            mChunk = first;
        }
        else
        {
            current->next = nullptr;
            mChunk = first;
        }

    #if DEBUG
        profile("expand");
    #endif
    }

#if DEBUG
    void ListAllocator::profile(const char* msg)
    {

    }
#endif

}