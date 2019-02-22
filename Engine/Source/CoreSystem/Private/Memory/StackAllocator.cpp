//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Misc/Assert.h"
#include "Misc/Alignment.h"
#include "Memory/Allocator.h"
#include "Memory/StackAllocator.h"

namespace Berserk
{

    StackAllocator::StackAllocator(uint32 size)
    {
        FAIL(size >= MIN_BUFFER_SIZE, "Buffer size must be more than minimum size %u", MIN_BUFFER_SIZE);
        ALIGN(size);

        mUsage = 0;
        mTotalSize = size;

        mBuffer = (Data*) Allocator::getSingleton().memoryAllocate(size);
    }

    StackAllocator::~StackAllocator()
    {
        if (mBuffer)
        {
            free();

            Allocator::getSingleton().memoryFree(mBuffer);
            mBuffer = nullptr;

            printf("Stack allocator: delete buffer %u\n", mTotalSize);
        }
    }

    void* StackAllocator::alloc(uint32 size)
    {
        ALIGN(size);
        FAIL(mUsage + size + sizeof(Data) <= mTotalSize, "Cannot allocate memory. Buffer is full");

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

        mUsage += size + sizeof(Data);

        return pointer;
    }

    void StackAllocator::free(void *pointer)
    {
        FAIL(pointer == (uint8*)mBuffer + sizeof(Data), "An attempt to free not previously allocated chunk of memory");

        mUsage -= mBuffer->size + sizeof(Data);
        mBuffer = mBuffer->prev;
    }

    void StackAllocator::free()
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

    uint32 StackAllocator::getTotalSize() const
    {
        return mTotalSize;
    }

}