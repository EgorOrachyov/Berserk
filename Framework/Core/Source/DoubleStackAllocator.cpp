//
// Created by Egor Orachyov on 25.05.2018.
//

#include "../Memory/DoubleStackAllocator.h"
#include "../Memory/MemoryAllocators.h"
#include "../Logging/LogMessages.h"
#include "../Essential/Assert.h"

namespace Berserk
{

    DoubleStackAllocator::DoubleStackAllocator()
    {
        mHead = 0;
        mTail = 0;
        mHeadMarker = 0;
        mTailMarker = 0;
        mSize = 0;
        mBuffer = NULL;
    }

    DoubleStackAllocator::~DoubleStackAllocator()
    {
        PUSH("Delete double stack allocator %p buffer %p\n", this, mBuffer);
        destroy();
    }

    void DoubleStackAllocator::init(UINT32 size)
    {
        ASSERT(size >= 64, "Buffer size cannot be less than 64 bytes");
        ASSERT(!mBuffer, "Buffer should be NULL before init");

        mSize = size;
        mTail = mSize;
        mTailMarker = mTail;
        mBuffer = mem_alloc(mSize);
    }

    void DoubleStackAllocator::destroy()
    {
        if (mBuffer)
        {
            mem_free(mBuffer);
        }

        mHead = 0;
        mTail = 0;
        mHeadMarker = 0;
        mTailMarker = 0;
        mSize = 0;
        mBuffer = NULL;
    }

    void* DoubleStackAllocator::allocHead(UINT32 size)
    {
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));
        ASSERT(mHead + size <= mTail, "Buffer is full, cannot allocate memory");

        UINT32 offset = mHead + size;

        if (offset > mTail)
        {
            return NULL;
        }

        void* ptr = (void*)((INT8*)mBuffer + mHead);
        mHead = offset;

        return ptr;
    }

    void* DoubleStackAllocator::allocTail(UINT32 size)
    {
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));
        ASSERT(mHead <= mTail - size, "Buffer is full, cannot allocate memory");

        UINT32 offset = mTail - size;

        if (mHead > offset)
        {
            return NULL;
        }

        void* ptr = (void*)((INT8*)mBuffer + offset);
        mTail = offset;

        return ptr;
    }

    void* DoubleStackAllocator::callocHead(UINT32 count, UINT32 size)
    {
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));
        ASSERT(mHead + count * size <= mTail, "Buffer is full, cannot allocate memory");

        UINT32  offset = mHead + count * size;

        if (offset > mTail)
        {
            return NULL;
        }

        void* ptr = (void*)((INT8*)mBuffer + mHead);
        mHead = offset;

        return ptr;
    }

    void* DoubleStackAllocator::callocTail(UINT32 count, UINT32 size)
    {
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));
        ASSERT(mHead <= mTail - count * size, "Buffer is full, cannot allocate memory");

        UINT32  offset = mTail - count * size;

        if (mHead > offset)
        {
            return NULL;
        }

        void* ptr = (void*)((INT8*)mBuffer + offset);
        mTail = offset;

        return ptr;
    }

    void DoubleStackAllocator::resetHead()
    {
        mHead = 0;
    }

    void DoubleStackAllocator::resetTail()
    {
        mTail = mSize;
    }

    void DoubleStackAllocator::setMarkerHead()
    {
        mHeadMarker = mHead;
    }

    void DoubleStackAllocator::setMarkerTail()
    {
        mTailMarker = mTail;
    }

    void DoubleStackAllocator::free()
    {
        resetHead();
        resetTail();
    }

    void DoubleStackAllocator::freeToHeadMarker()
    {
        mHead = mHeadMarker;
    }

    void DoubleStackAllocator::freeToTailMarker()
    {
        mTail = mTailMarker;
    }



} // namespace Berserk