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
            : mPool(Math::max(chunkSize, MIN_CHUNK_SIZE), CHUNK_COUNT)
    {
        ASSERT(chunkSize >= MIN_CHUNK_SIZE , "Chunk size must be more than %u", MIN_CHUNK_SIZE);
        ALIGN(chunkSize);

        mUsage = 0;
        mChunks = nullptr;
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
        return CHUNK_COUNT;
    }

    uint32 ListAllocator::getBufferSize() const
    {
        return CHUNK_COUNT * mChunkSize;
    }

    uint32 ListAllocator::getUsage() const
    {
        return mUsage;
    }

    uint32 ListAllocator::getTotalSize() const
    {
        return mPool.getTotalSize();
    }

}