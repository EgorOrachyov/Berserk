//
// Created by Egor Orachyov on 25.05.2018.
//

#include "../Core/Memory/StackAllocator.h"
#include "../Core/Essential/Assert.h"
#include "../Core/Memory/MemoryAllocators.h"

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

    inline void StackAllocator::Init(uint32 size)
    {
        ASSERT(size >= 64, "Buffer size cannot be less than 64 bytes");

        mTop = 0;
        mMarker = 0;
        mSize = size;

        ASSERT(!mBuffer, "Buffer should be NULL before init");
        mBuffer = mem_alloc(mSize);
    }

    inline void StackAllocator::Reset()
    {
        if (mBuffer)
        { mem_free(mBuffer); }
        mSize = 0;
        mTop = 0;
        mMarker = 0;
        mBuffer = NULL;
    }

    void StackAllocator::ReInit(uint32 size)
    {
        Reset();
        Init(size);
    }

    inline void* StackAllocator::Alloc(uint32 size)
    {
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));
        ASSERT(mTop + size <= mSize, "Buffer is full, cannot allocate memory");

        uint32 offset = mTop + size;
        void* ptr = (void*)((int8*)mBuffer + offset);
        mTop = offset;

        return ptr;
    }

    inline void* StackAllocator::Calloc(uint32 count, uint32 size)
    {
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));
        ASSERT(mTop + count * size <= mSize, "Buffer is full, cannot allocate memory");

        uint32  offset = mTop + count * size;
        void* ptr = (void*)((int8*)mBuffer + offset);
        mTop = offset;

        return ptr;
    }

    inline void* StackAllocator::Alloc(uint32 size, uint8 alignment)
    {
        size = size + ((size & (alignment - 1)) != 0) * (alignment - (size % alignment));
        ASSERT((alignment - 1) & alignment == 0, "Alignment should be power of 2")
        ASSERT(mTop + size <= mSize, "Buffer is full, cannot allocate memory");

        uint32 offset = mTop + size;
        void* ptr = (void*)((int8*)mBuffer + offset);
        mTop = offset;


        return ptr;
    }

    inline void* StackAllocator::Calloc(uint32 count, uint32 size, uint8 alignment)
    {
        size = size + ((size & (alignment - 1)) != 0) * (alignment - (size % alignment));
        ASSERT(((alignment - 1) & alignment) == 0, "Alignment should be power of 2")
        ASSERT(mTop + count * size <= mSize, "Buffer is full, cannot allocate memory");

        uint32  offset = mTop + count * size;
        void* ptr = (void*)((int8*)mBuffer + offset);
        mTop = offset;

        return ptr;
    }

    inline void StackAllocator::SetMarker()
    {
        mMarker = mTop;
    }

    inline void StackAllocator::FreeToMarker()
    {
        mTop = mMarker;
    }

    inline void StackAllocator::Free()
    {
        mTop = 0;
    }

} // namespace Berserk