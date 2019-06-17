//
// Created by Egor Orachyov on 24.03.2019.
//

#include "Memory/Old/ProxyAllocator.h"
#include "Misc/Assert.h"

namespace Berserk
{

    ProxyAllocator::ProxyAllocator(IAllocator *allocator) : mAllocator(allocator)
    {
        FAIL(allocator, "Null pointer IAllocator")
    }

    void* ProxyAllocator::allocate(uint32 size)
    {
        mAllocCalls += 1;
        mTotalMemUsage += size;
        return mAllocator->allocate(size);
    }

    void ProxyAllocator::free(void *pointer)
    {
        mFreeCalls += 1;
        mAllocator->free(pointer);
    }

} // namespace Berserk