//
// Created by Egor Orachyov on 02.02.2019.
//

#include "Misc/Assert.h"
#include "Strings/StringPool.h"

namespace Berserk
{
    StringPool::StringPool()
    {
        uint32 count[] = {32, 16, 8, 4, 2, 1};

        for (uint32 i = 0; i < Supported; i++)
        {
            new(&mPool[i]) PoolAllocator(getChunkSize(POOL_STRING_SIZES[i]), count[i]);
        }
    }

    StringPool::PoolNode* StringPool::create()
    {
        return &mCreateNode;
    }

    StringPool::PoolNode * StringPool::allocate(uint32 size)
    {
        auto index = getBestFit(size);

        auto node = (PoolNode*) mPool[index].alloc();
        node->mSize = POOL_STRING_SIZES[index];
        node->mLength = 0;
        node->mReferenceCount = 1;

        return node;
    }

    void StringPool::free(PoolNode *node)
    {
        if (node->mSize == 0)
        {
            return;
        }

        if (node->mReferenceCount > 0)
        {
            node->mReferenceCount -= 1;
        }

        if (node->mReferenceCount == 0)
        {
            auto index = getBufferIndex(node->mSize);
            mPool[index].free(node);
        }
    }

    StringPool& StringPool::getSingleton()
    {
        static StringPool globalStringPool;
        return globalStringPool;
    }

    void* StringPool::getBufferPtr(PoolNode *node)
    {
        return (((uint8*)node) + NODE_INFO_OFFSET);
    }

    void* StringPool::getNodePtr(void *buffer)
    {
        return (((uint8*)buffer) - NODE_INFO_OFFSET);
    }

    uint32 StringPool::getBestFit(uint32 size)
    {
        for (uint32 i = 0; i < Supported; i++)
        {
            if (size < POOL_STRING_SIZES[i])
            {
                return i;
            }
        }

        FAIL(false, "Unsupported string buffer size [%u]", size);
    }

    uint32 StringPool::getBufferIndex(uint32 size)
    {
        for (uint32 i = 0; i < Supported; i++)
        {
            if (size == POOL_STRING_SIZES[i])
            {
                return i;
            }
        }

        FAIL(false, "Unsupported string buffer size [%u]", size);
    }

    uint32 StringPool::getChunkSize(uint32 bufferSize)
    {
        return NODE_INFO_OFFSET + bufferSize;
    }

    const uint16 StringPool::POOL_STRING_SIZES[StringPool::Supported]
            = {Length32, Length64, Length128, Length256, Length512, Length1024};

} // namespace Berserk