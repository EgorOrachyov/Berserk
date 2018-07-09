//
// Created by Egor Orachyov on 25.05.2018.
//

#include "../Memory/DoubleFrameAllocator.h"

namespace Berserk
{

    DoubleFrameAllocator::DoubleFrameAllocator()
    {
        mCurrent = NULL;
        mPrevious = NULL;
    }

    DoubleFrameAllocator::~DoubleFrameAllocator()
    {
        reset();
    }

    void DoubleFrameAllocator::init(UINT32 size)
    {
        mFirstAlloc.init(size);
        mSecondAlloc.init(size);
        mCurrent = &mFirstAlloc;
        mPrevious = &mSecondAlloc;
    }

    void DoubleFrameAllocator::swap()
    {
        StackAllocator* tmp = mCurrent;
        mCurrent = mPrevious;
        mPrevious = tmp;

        mCurrent->free();
    }

    void DoubleFrameAllocator::reset()
    {
        mFirstAlloc.reset();
        mSecondAlloc.reset();
        mCurrent = NULL;
        mPrevious = NULL;
    }

    void DoubleFrameAllocator::reInit(UINT32 size)
    {
        reset();
        init(size);
    }

    void* DoubleFrameAllocator::allocBlock(UINT32 size)
    {
        return mCurrent->allocBlock(size);
    }

    void* DoubleFrameAllocator::callocBlock(UINT32 count, UINT32 size)
    {
        return mCurrent->callocBlock(count, size);
    }

    void* DoubleFrameAllocator::allocBlock(UINT32 size, UINT8 alignment)
    {
        return mCurrent->allocBlock(size, alignment);
    }

    void* DoubleFrameAllocator::callocBlock(UINT32 count, UINT32 size, UINT8 alignment)
    {
        return mCurrent->callocBlock(count, size, alignment);
    }

    void DoubleFrameAllocator::free()
    {
        mFirstAlloc.free();
        mSecondAlloc.free();
    }

}