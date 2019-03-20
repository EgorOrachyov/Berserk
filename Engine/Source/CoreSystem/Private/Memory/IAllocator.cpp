//
// Created by Egor Orachyov on 20.03.2019.
//

#include "Memory/IAllocator.h"

namespace Berserk
{
    IAllocator::IAllocator() : mAllocCalls(0),
                               mFreeCalls(0),
                               mTotalMemUsage(0)
    {

    }

    uint32 IAllocator::getFreeCalls() const
    {
        return mFreeCalls;
    }

    uint32 IAllocator::getAllocateCalls() const
    {
        return mAllocCalls;
    }

    uint64 IAllocator::getTotalMemoryUsage() const
    {
        return mTotalMemUsage;
    }

}