//
// Created by Egor Orachyov on 26.05.2018.
//

#ifndef BERSERKENGINE_POOLALLOCATOR_H
#define BERSERKENGINE_POOLALLOCATOR_H

#include "../Types.h"
#include "../Common.h"
#include "../MemoryProfiler.h"
#include "../MemoryAllocators.h"
#include "../Assert.h"

namespace Berserk
{

    class PoolAllocator
    {
    public:

        PoolAllocator()
        {
            mElementSize = 0;
            mElementsCount = 0;
            mTotalCount = 0;
            mExpandFactor = 2;
            mBuffer = NULL;
            mOffsetToHead = NULL;
            mOffsetToLast = NULL;
        }

        ~PoolAllocator()
        {
            if (mBuffer)
            { FreeMemory(mBuffer); }
        }

        inline void Init(uint32 elementSize, uint32 maxElementsCount = 64, uint8 expandFactor = 2)
        {
            ASSERT(elementSize >= 8, "Element's size cannot be less than 8 bytes");
            ASSERT(maxElementsCount >= 16, "Element's count cannot be less than 16");
            ASSERT(expandFactor >= 2, "Expand factor cannot be less than 2");

            mElementSize = elementSize + (uint32)(MEMORY_ALIGNMENT - (elementSize % MEMORY_ALIGNMENT));
            mTotalCount = maxElementsCount;
            mExpandFactor = expandFactor;
        }

        inline void* GetBlock()
        {
            ASSERT(mElementsCount >= mTotalCount, "No available block to allocate");

            return NULL;
        }

    public:

        inline void SetUpBuffer()
        {
            mBuffer = CAlloc(mTotalCount, mElementSize);
            mOffsetToHead = 0;

            uint32 offset = mOffsetToHead;
            for(uint32 i = 0; i < mTotalCount - 1; i++)
            {
                int64 current = (int64)mBuffer + offset;
                offset += mElementSize;
                int64 next = (int64)mBuffer + offset;
                int64* ptr = (int64*)current;
                *ptr = next;
            }

            mOffsetToLast = offset;

            int64 current = (int64)mBuffer + offset;
            int64 next = NULL;
            int64* ptr = (int64*)current;
            *ptr = next;

        }

        inline void ExpandBuffer()
        {
            uint32 newTotalCount = mTotalCount * mExpandFactor;
            mBuffer = ReAlloc(mBuffer, newTotalCount);



            mTotalCount = newTotalCount;
        }

        inline void PrintBuffer()
        {
            printf("Pool Allocator [Debug]\n");

            for(uint32 i = 0; i < mTotalCount; i++)
            {
                int64 current = (int64)mBuffer + i * mElementSize;
                int64* ptr = (int64*)current;
                printf("[index: %3i] current %p next %lx \n", i, ptr, *ptr);
            }

            printf("Pool Allocator [Debug]\n\n");
        }

    private:

        uint32 mElementSize;
        uint32 mElementsCount;
        uint32 mTotalCount;
        uint32 mOffsetToHead;
        uint32 mOffsetToLast;
        uint8  mExpandFactor;
        void*  mBuffer;

    };

} // namespace Berserk

#endif //BERSERKENGINE_POOLALLOCATOR_H
