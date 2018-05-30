//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_STACKALLOCATOR_H
#define BERSERKENGINE_STACKALLOCATOR_H

#include "../Types.h"
#include "../Common.h"
#include "../Assert.h"
#include "../MemoryAllocators.h"
#include "../UsageDescriptors.h"

namespace Berserk
{

    /**
     *
     */
    class MEM_API StackAllocator
    {
    public:

        StackAllocator()
        {
            mSize = 0;
            mTop = 0;
            mMarker = 0;
            mBuffer = NULL;
        }

        ~StackAllocator()
        {
            if (mBuffer)
            { mem_free(mBuffer); }
        }

        /**
         *
         * @param size
         */
        inline void Init(uint32 size = 64)
        {
            ASSERT(size >= 64, "Buffer size cannot be less than 64 bytes");

            mTop = 0;
            mMarker = 0;
            mSize = size;

            ASSERT(!mBuffer, "Buffer should be NULL before init");

            mBuffer = mem_alloc(mSize);
        }

        /**
         *
         */
        inline void Reset()
        {
            if (mBuffer)
            { mem_free(mBuffer); }

            mSize = 0;
            mTop = 0;
            mMarker = 0;
            mBuffer = NULL;
        }

        /**
         *
         * @param size
         */
        inline void ReInit(uint32 size = 64)
        {
            Reset();
            Init(size);
        }

        /**
         *
         * @param size
         * @return
         */
        inline void* Alloc(uint32 size)
        {
            ASSERT(mTop + size <= mSize, "Buffer is full, cannot allocate memory");

            uint32 offset = mTop + size;
            void* ptr = (void*)((int8*)mBuffer + offset);
            mTop = offset;

            return ptr;
        }

        /**
         *
         * @param count
         * @param size
         * @return
         */
        inline void* Calloc(uint32 count, uint32 size)
        {
            ASSERT(mTop + count * size <= mSize, "Buffer is full, cannot allocate memory");

            uint32  offset = mTop + count * size;
            void* ptr = (void*)((int8*)mBuffer + offset);
            mTop = offset;

            return ptr;
        }

        /**
         *
         * @param size
         * @param alignment
         * @return
         */
        inline void* Alloc(uint32 size, uint8 alignment)
        {
            size = size + (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));

            ASSERT((alignment - 1) & alignment == 0, "Alignment should be power of 2")
            ASSERT(mTop + size <= mSize, "Buffer is full, cannot allocate memory");

            uint32 offset = mTop + size;
            void* ptr = (void*)((int8*)mBuffer + offset);
            mTop = offset;

            return ptr;
        }

        /**
         *
         * @param count
         * @param size
         * @param alignment
         * @return
         */
        inline void* Calloc(uint32 count, uint32 size, uint8 alignment)
        {
            size = size + (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));

            ASSERT((alignment - 1) & alignment == 0, "Alignment should be power of 2")
            ASSERT(mTop + count * size <= mSize, "Buffer is full, cannot allocate memory");

            uint32  offset = mTop + count * size;
            void* ptr = (void*)((int8*)mBuffer + offset);
            mTop = offset;

            return ptr;
        }

        /**
         *
         */
        inline void SetMarker()
        {
            mMarker = mTop;
        }

        /**
         *
         */
        inline void FreeToMarker()
        {
            mTop = mMarker;
        }

        /**
         *
         */
        inline void Free()
        {
            mTop = 0;
        }

    private:

        uint32 mSize;
        uint32 mTop;
        uint32 mMarker;
        void* mBuffer;

    };


} // namespace Berserk

#endif //BERSERKENGINE_STACKALLOCATOR_H
