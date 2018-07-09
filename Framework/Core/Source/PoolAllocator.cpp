//
// Created by Egor Orachyov on 01.06.2018.
//

#include "Memory/PoolAllocator.h"
#include "../Profiling/MemoryProfiler.h"
#include "../Memory/MemoryAllocators.h"
#include "../Math/UtilityNumbers.h"

namespace Berserk
{

    PoolAllocator::PoolAllocator()
    {
        mElementSize = 0;
        mOneBufferCount = 0;
        mTotalCount = 0;
        mCurrentCount = 0;
        mIsStatic = 0;
        mHead = NULL;
        mBuffer = NULL;
    }

    PoolAllocator::~PoolAllocator()
    {
        PUSH("Delete pool allocator %p buffers from %p\n", this, mBuffer);
        reset();
    }

    void PoolAllocator::init(uint16 elementSize, uint16 elementsCountInOneBuffer)
    {
        ASSERT(elementSize >= sizeof(Node), "Element's size cannot be less than List Node size");
        ASSERT(elementsCountInOneBuffer >= 16, "Element's count in one buffer cannot be less than 16");

        elementsCountInOneBuffer = (uint16)max(16, (uint16)elementsCountInOneBuffer);
        elementSize = (uint16)max(elementSize, (uint16)sizeof(Node));

        mElementSize = elementSize + (uint32)((elementSize & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (elementSize % MEMORY_ALIGNMENT));
        mOneBufferCount = elementsCountInOneBuffer;
        mTotalCount = elementsCountInOneBuffer;
        mCurrentCount = mTotalCount;

        mIsStatic = 0;

        // Pre-mem_alloc memory for future allocations
        setUpBuffer();
    }

    void PoolAllocator::reset()
    {
        Buffer* tmp = mBuffer;
        while (tmp)
        {
            tmp = mBuffer->next; // printf("Free %p \n", mBuffer);
            mem_free(mBuffer);
            mBuffer = tmp;
        }

        mElementSize = 0;
        mTotalCount = 0;
        mOneBufferCount = 0;
        mIsStatic = 0;
        mCurrentCount = 0;
        mHead = NULL;
        mBuffer = NULL;
    }

    void PoolAllocator::reInit(uint16 elementSize, uint16 elementsCountInOneBuffer)
    {
        reset();
        init(elementSize, elementsCountInOneBuffer);
    }

    void* PoolAllocator::allocBlock()
    {
        if (mHead == NULL)
        {
            if (mIsStatic) return NULL;
            else expandBuffer();
        }

        void* ptr = (void*)mHead;
        mHead = mHead->next;

        mCurrentCount -= 1;

        return ptr;
    }

    void PoolAllocator::freeBlock(void *block)
    {
        Node* node = mHead;
        mHead = (Node*)block;
        mHead->next = node;

        mCurrentCount += 1;
    }

    uint16 PoolAllocator::getCapacity() const
    {
        return mTotalCount;
    }

    uint16 PoolAllocator::getNumOfFreeBlocks() const
    {
        return mCurrentCount;
    }

    uint32 PoolAllocator::getOneBufferCapacity() const
    {
        return mOneBufferCount;
    }

    uint32 PoolAllocator::getElementSize() const
    {
        return mElementSize;
    }

    bool PoolAllocator::isStatic() const
    {
        return (mIsStatic != 0);
    }

    void PoolAllocator::setStatic(bool lookExpanding)
    {
        mIsStatic = (uint16)lookExpanding;
    }

    void PoolAllocator::setOneBufferCapacity(uint32 capacity)
    {
        mOneBufferCount = (uint32)max((uint32)16, (uint32)capacity);
    }

    void PoolAllocator::setUpBuffer()
    {
        ASSERT(!mBuffer, "Buffer should be NULL before init");

        mBuffer = (Buffer*)mem_alloc(mOneBufferCount * mElementSize + sizeof(Buffer*));
        mBuffer->next = NULL;
        mHead = (Node*)((int8*)mBuffer + sizeof(Buffer*));

        Node* current = mHead;
        for(uint32 i = 0; i < mOneBufferCount - 1; i++)
        {
            Node* next = (Node*)((int8*)current + mElementSize);
            current->next = next;
            current = next;
        }

        current->next = NULL;
    }

    void PoolAllocator::expandBuffer()
    {
        Buffer* tmp = mBuffer;

        mBuffer = (Buffer*)mem_alloc(mOneBufferCount * mElementSize + sizeof(Buffer*));
        mBuffer->next = tmp;
        mHead = (Node*)((int8*)mBuffer + sizeof(Buffer*));

        Node* current = (Node*)((int8*)mBuffer + sizeof(Buffer*));
        for(uint32 i = 0; i < mOneBufferCount - 1; i++)
        {
            Node* next = (Node*)((int8*)current + mElementSize);
            current->next = next;
            current = next;
        }

        current->next = NULL;

        mCurrentCount += mOneBufferCount;
        mTotalCount += mOneBufferCount;
    }

    void PoolAllocator::printInfo()
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

