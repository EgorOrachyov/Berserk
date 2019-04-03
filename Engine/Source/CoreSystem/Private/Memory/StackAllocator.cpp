//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Misc/Assert.h"
#include "Misc/Alignment.h"
#include "Memory/Allocator.h"
#include "Memory/StackAllocator.h"

namespace Berserk
{

    StackAllocator::StackAllocator(uint32 size, IAllocator* allocator)
    {
        FAIL(size >= MIN_BUFFER_SIZE, "Buffer size must be more than minimum size %u", MIN_BUFFER_SIZE);
        ALIGN(size);

        mUsage = 0;
        mTotalMemUsage = size;

        if (allocator) mAllocator = allocator;
        else mAllocator = &Allocator::getSingleton();

        mBuffer = (Data*) mAllocator->allocate(size);
    }

    StackAllocator::~StackAllocator()
    {
        if (mBuffer)
        {
            clear();

            mAllocator->free(mBuffer);
            mBuffer = nullptr;

#if PROFILE_STACK_ALLOCATOR
            printf("Stack allocator: delete buffer %lu\n", mTotalMemUsage);
#endif
        }
    }

    void* StackAllocator::allocate(uint32 size)
    {
        ALIGN(size);
        FAIL(mUsage + size + sizeof(Data) <= mTotalMemUsage, "Cannot allocate memory. Buffer is full");

        void* pointer;

        if (mUsage == 0)
        {
            mBuffer->prev = mBuffer;
            mBuffer->size = size;

            pointer = (uint8*)mBuffer + sizeof(Data);
        }
        else
        {
            auto chunk = mBuffer;
            chunk = (Data*)((uint8*)chunk + chunk->size + sizeof(Data));
            chunk->prev = mBuffer;
            chunk->size = size;

            mBuffer = chunk;

            pointer = (uint8*)mBuffer + sizeof(Data);
        }

        mAllocCalls += 1;
        mUsage += size + sizeof(Data);

        return pointer;
    }

    void StackAllocator::free(void *pointer)
    {
        FAIL(pointer == (uint8*)mBuffer + sizeof(Data), "An attempt to free not previously allocated chunk of memory");

        mUsage -= mBuffer->size + sizeof(Data);
        mBuffer = mBuffer->prev;

        mFreeCalls += 1;
    }

    void StackAllocator::clear()
    {
        if (mUsage)
        {
            mBuffer = (Data*) ((uint8*)mBuffer - (mUsage - mBuffer->size - sizeof(Data)));
            mUsage = 0;
        }
    }

    uint32 StackAllocator::getUsage() const
    {
        return mUsage;
    }

}