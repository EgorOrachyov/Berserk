//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Misc/Assert.h"
#include "Misc/Alignment.h"
#include "Memory/Allocator.h"
#include "Memory/LinearAllocator.h"

namespace Berserk
{

    LinearAllocator::LinearAllocator(uint32 size, IAllocator* allocator)
    {
        FAIL(size >= MIN_BUFFER_SIZE, "Buffer size must be more than minimum size %u", MIN_BUFFER_SIZE);
        ALIGN(size);

        mUsage = 0;
        mTotalMemUsage = size;

        if (allocator) mAllocator = allocator;
        else mAllocator = &Allocator::get();

        mBuffer = mAllocator->allocate(size);
    }

    LinearAllocator::~LinearAllocator()
    {
        if (mBuffer)
        {
            mAllocator->free(mBuffer);
            mBuffer = nullptr;

#if PROFILE_LINEAR_ALLOCATOR
            printf("Linear allocator: delete buffer %lu\n", mTotalMemUsage);
#endif
        }
    }

    void* LinearAllocator::allocate(uint32 size)
    {
        ALIGN(size);
        FAIL(mUsage + size <= mTotalMemUsage, "Cannot allocate memory. Buffer is full");

        auto pointer = (void*)((uint8*)mBuffer + mUsage);
        mUsage += size;

        mAllocCalls += 1;

        return pointer;
    }

    void LinearAllocator::clear()
    {
        mUsage = 0;
    }

    uint32 LinearAllocator::getUsage() const
    {
        return mUsage;
    }

}