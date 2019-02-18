//
// Created by Egor Orachyov on 02.02.2019.
//

#include "Public/Strings/StringPool.h"
#include "Public/Strings/StringStream.h"

namespace Berserk
{
    uint16 sizes[]  = {32, 64, 128, 256, 512, 1024};
    uint16 counts[] = {32, 16, 8,   4,   2,   1};

    struct HashNode
    {
        uint16 mSize;               // Total buffer size
        uint16 mLength;             // Number of used symbols without \0
        uint32 mReferenceCount;     // Number of references to this string
    };

    uint32 CNodeSize(uint32 bufferSize)
    {
        return sizeof(HashNode) + sizeof(char) * bufferSize;
    }

    uint32 WNodeSize(uint32 bufferSize)
    {
        return sizeof(HashNode) + sizeof(wchar_t) * bufferSize;
    }

    StringPool::StringPool()
    {
        for(uint32 i = 0; i < StringSizes::Supported; i++)
        {
            auto p = new(&mCPool[i]) PoolAllocator(CNodeSize(sizes[i]), counts[i]);
            auto q = new(&mWPool[i]) PoolAllocator(WNodeSize(sizes[i]), counts[i]);
        }
    }

    void* StringPool::allocC(StringSizes size)
    {
        auto node = (HashNode*) mCPool[size].alloc();

        node->mSize = sizes[size];
        node->mLength = 0;
        node->mReferenceCount += 1;

        return node;
    }

    void* StringPool::allocW(StringSizes size)
    {
        auto node = (HashNode*) mWPool[size].alloc();

        node->mSize = sizes[size];
        node->mLength = 0;
        node->mReferenceCount += 1;

        return node;
    }

    void StringPool::freeC(StringSizes size, void *pointer)
    {
        auto node = (HashNode*) pointer;
        if (--node->mReferenceCount == 0)
        {
            mCPool[size].free(pointer);
        }
    }

    void StringPool::freeW(StringSizes size, void *pointer)
    {
        auto node = (HashNode*) pointer;
        if (--node->mReferenceCount == 0)
        {
            mWPool[size].free(pointer);
        }
    }

    StringPool& StringPool::getSingleton()
    {
        static StringPool stringPool;
        return stringPool;
    }

} // namespace Berserk