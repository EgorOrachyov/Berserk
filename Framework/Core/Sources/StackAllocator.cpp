//
// Created by Egor Orachyov on 25.05.2018.
//

#include "../Memory/StackAllocator.h"
#include "../Essential/Assert.h"
#include "../Memory/MemoryAllocators.h"

namespace Berserk
{
    StackAllocator::StackAllocator()
    {
        mSize = 0;
        mTop = 0;
        mMarker = 0;
        mBuffer = NULL;
    }

    StackAllocator::~StackAllocator()
    {
        if (mBuffer)
        { mem_free(mBuffer); }
    }

    void StackAllocator::init(uint32 size)
    {
        ASSERT(size >= 64, "Buffer size cannot be less than 64 bytes");

        mTop = 0;
        mMarker = 0;
        mSize = size;

        ASSERT(!mBuffer, "Buffer should be NULL before init");
        mBuffer = mem_alloc(mSize);
    }

    void StackAllocator::reset()
    {
        if (mBuffer)
        { mem_free(mBuffer); }
        mSize = 0;
        mTop = 0;
        mMarker = 0;
        mBuffer = NULL;
    }

    void StackAllocator::reInit(uint32 size)
    {
        reset();
        init(size);
    }

    void* StackAllocator::allocBlock(uint32 size)
    {
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));
        ASSERT(mTop + size <= mSize, "Buffer is full, cannot allocate memory");

        uint32 offset = mTop + size;

        if (offset > mSize) {
            return NULL;
        }

        void* ptr = (void*)((int8*)mBuffer + offset);
        mTop = offset;

        return ptr;
    }

    void* StackAllocator::callocBlock(uint32 count, uint32 size)
    {
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));
        ASSERT(mTop + count * size <= mSize, "Buffer is full, cannot allocate memory");

        uint32  offset = mTop + count * size;

        if (offset > mSize) {
            return NULL;
        }

        void* ptr = (void*)((int8*)mBuffer + offset);
        mTop = offset;

        return ptr;
    }

    void* StackAllocator::allocBlock(uint32 size, uint8 alignment)
    {
        size = size + ((size & (alignment - 1)) != 0) * (alignment - (size % alignment));
        ASSERT((alignment - 1) & alignment == 0, "Alignment should be power of 2")
        ASSERT(mTop + size <= mSize, "Buffer is full, cannot allocate memory");

        uint32 offset = mTop + size;

        if (offset > mSize) {
            return NULL;
        }

        void* ptr = (void*)((int8*)mBuffer + offset);
        mTop = offset;


        return ptr;
    }

    void* StackAllocator::callocBlock(uint32 count, uint32 size, uint8 alignment)
    {
        size = size + ((size & (alignment - 1)) != 0) * (alignment - (size % alignment));
        ASSERT(((alignment - 1) & alignment) == 0, "Alignment should be power of 2")
        ASSERT(mTop + count * size <= mSize, "Buffer is full, cannot allocate memory");

        uint32  offset = mTop + count * size;

        if (offset > mSize) {
            return NULL;
        }

        void* ptr = (void*)((int8*)mBuffer + offset);
        mTop = offset;

        return ptr;
    }

    void StackAllocator::setMarker()
    {
        mMarker = mTop;
    }

    void StackAllocator::freeToMarker()
    {
        mTop = mMarker;
    }

    void StackAllocator::free()
    {
        mTop = 0;
    }

} // namespace Berserk