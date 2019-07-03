//
// Created by Egor Orachyov on 03.07.2019.
//

#include "LinearAllocator.h"
#include <Misc/AssertDev.h>
#include <Misc/Alignment.h>

namespace Berserk
{

    LinearAllocator::LinearAllocator(uint32 bufferSize, IAllocator &allocator)
            : mAllocator(allocator), mBufferSize(bufferSize)
    {
        assertion_dev(bufferSize > 0);
        mBuffer = (uint8*) mAllocator.allocate(bufferSize);
        mTotalMemoryUsage = mBufferSize;
    }

    LinearAllocator::~LinearAllocator()
    {
        if (mBuffer != nullptr)
        {
#ifdef DEBUG
            char buffer[20];
            printf("LinearAllocator: [allocation calls: %u] [free calls: %u] [total memory usage: %s]\n",
                   mAllocateCalls, mFreeCalls, Printer::print((uint32)getTotalMemoryUsage(), buffer));
#endif
            mAllocator.free(mBuffer);
            mBuffer = nullptr;
            mBufferSize = 0;
            mCurrentUsage = 0;
        }
    }

    void *LinearAllocator::allocate(uint32 size)
    {
        return allocate(size, MEMORY_ALIGNMENT);
    }

    void *LinearAllocator::allocate(uint32 size, uint32 alignment)
    {
        uint32 oldSize = size;

        ALIGNMENT_PARAM(size, alignment);
        assertion_dev_msg(canAllocate(size),
                          "LinearAllocator: cannot allocate memory [size: %u] [alignment: %u] [current usage: %u] [buffer size: &u]",
                          oldSize, alignment, mCurrentUsage, mBufferSize);

        uint8* result = mBuffer + mCurrentUsage;
        mCurrentUsage += size;
        mAllocateCalls += 1;

        return result;
    }

    void LinearAllocator::free(void *pointer)
    {
        /** Do actually nothing */
        mFreeCalls += 1;
    }

    void LinearAllocator::clear()
    {
        mCurrentUsage = 0;
    }

    bool LinearAllocator::canAllocate(uint32 size) const
    {
        return size + mCurrentUsage <= mBufferSize;
    }

} // namespace Berserk