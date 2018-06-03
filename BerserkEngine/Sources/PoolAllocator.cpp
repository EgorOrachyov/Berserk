//
// Created by Egor Orachyov on 01.06.2018.
//

#include "../Core/Memory/PoolAllocator.h"
#include "../Core/Profiling/MemoryProfiler.h"
#include "../Core/MemoryAllocators.h"

namespace Berserk
{

    PoolAllocator::PoolAllocator()
    {
        mElementSize = 0;
        mTotalCount = 0;
        mHead = NULL;
        mBuffer = NULL;
    }

    PoolAllocator::~PoolAllocator()
    {
        if (mBuffer)
        { mem_free(mBuffer); }
    }

    void PoolAllocator::Init(uint32 elementSize, uint32 maxElementsCount)
    {
        ASSERT(elementSize >= 8, "Element's size cannot be less than 8 bytes");
        ASSERT(maxElementsCount >= 16, "Element's count cannot be less than 16");

        mElementSize = elementSize + (uint32)((elementSize & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (elementSize % MEMORY_ALIGNMENT));
        mTotalCount = maxElementsCount;

        // Pre-mem_alloc memory for future allocations
        SetUpBuffer();
    }

    void PoolAllocator::Reset()
    {
        if (mBuffer)
        { mem_free(mBuffer); }

        mElementSize = 0;
        mTotalCount = 0;
        mHead = NULL;
        mBuffer = NULL;
    }

    void PoolAllocator::ReInit(uint32 elementSize, uint32 maxElementsCount)
    {
        Reset();
        Init(elementSize, maxElementsCount);
    }

    void* PoolAllocator::AllocBlock()
    {
        ASSERT(mHead, "Head is NULL, cannot allocate block");

        void* ptr = (void*)mHead;
        mHead = mHead->next;

        return ptr;
    }

    void PoolAllocator::FreeBlock(void* block)
    {
        ASSERT(block >= mBuffer, "Block out of buffer range");
        ASSERT(block < (int8*)mBuffer + (mElementSize * mTotalCount), "Block out of buffer range");

        Node* node = mHead;
        mHead = (Node*)block;
        mHead->next = node;
    }

    uint32 PoolAllocator::GetCapacity()
    {
        return mTotalCount;
    }

    void PoolAllocator::SetUpBuffer()
    {
        ASSERT(!mBuffer, "Buffer should be NULL before init");

        mBuffer = mem_calloc(mTotalCount, mElementSize);
        mHead = (Node*)mBuffer;

        Node* current = mHead;
        for(uint32 i = 0; i < mTotalCount - 1; i++)
        {
            Node* next = (Node*)((int8*)current + mElementSize);
            current->next = next;
            current = next;
        }

        current->next = NULL;
    }

    void PoolAllocator::PrintInfo()
    {
        printf("Pool Allocator\nElement Size: %i\nTotal Count: %i\n", mElementSize, mTotalCount);

        int32 i = 0;
        Node* current = mHead;
        while (current)
        {
            printf("[%3i] curr: %p next: %p\n", i++, current, current->next);
            current = current->next;
        }
    }



} // namespace Berserk

