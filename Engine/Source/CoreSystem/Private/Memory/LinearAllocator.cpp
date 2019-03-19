//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Misc/Assert.h"
#include "Misc/Alignment.h"
#include "Memory/Allocator.h"
#include "Memory/LinearAllocator.h"

namespace Berserk
{

    LinearAllocator::LinearAllocator(uint32 size)
    {
        FAIL(size >= MIN_BUFFER_SIZE, "Buffer size must be more than minimum size %u", MIN_BUFFER_SIZE);
        ALIGN(size);

        mUsage = 0;
        mTotalSize = size;

        mBuffer = Allocator::getSingleton().allocate(size);
    }

    LinearAllocator::~LinearAllocator()
    {
        if (mBuffer)
        {
            Allocator::getSingleton().free(mBuffer);
            mBuffer = nullptr;

            printf("Linear allocator: delete buffer %u\n", mTotalSize);
        }
    }

    void* LinearAllocator::alloc(uint32 size)
    {
        ALIGN(size);
        FAIL(mUsage + size <= mTotalSize, "Cannot allocate memory. Buffer is full");

        auto pointer = (void*)((uint8*)mBuffer + mUsage);
        mUsage += size;

        return pointer;
    }

    void LinearAllocator::free()
    {
        mUsage = 0;
    }

    uint32 LinearAllocator::getUsage() const
    {
        return mUsage;
    }

    uint32 LinearAllocator::getTotalSize() const
    {
        return mTotalSize;
    }

}