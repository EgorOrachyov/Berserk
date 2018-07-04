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

    void DoubleFrameAllocator::init(uint32 size)
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

    void DoubleFrameAllocator::reInit(uint32 size)
    {
        reset();
        init(size);
    }

    void* DoubleFrameAllocator::allocBlock(uint32 size)
    {
        return mCurrent->allocBlock(size);
    }

    void* DoubleFrameAllocator::callocBlock(uint32 count, uint32 size)
    {
        return mCurrent->callocBlock(count, size);
    }

    void* DoubleFrameAllocator::allocBlock(uint32 size, uint8 alignment)
    {
        return mCurrent->allocBlock(size, alignment);
    }

    void* DoubleFrameAllocator::callocBlock(uint32 count, uint32 size, uint8 alignment)
    {
        return mCurrent->callocBlock(count, size, alignment);
    }

    void DoubleFrameAllocator::free()
    {
        mFirstAlloc.free();
        mSecondAlloc.free();
    }

}