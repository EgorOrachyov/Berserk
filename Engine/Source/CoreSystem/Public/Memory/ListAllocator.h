//
// Created by Egor Orachyov on 17.06.2019.
//

#ifndef BERSERK_LISTALLOCATOR_H
#define BERSERK_LISTALLOCATOR_H

#include <Memory/IAllocator.h>
#include <Memory/Allocator.h>

namespace Berserk
{

    class MEMORY_API ListAllocator : public IAllocator
    {
    public:

        explicit ListAllocator(uint32 bufferSize, IAllocator& allocator = Allocator::get());

        ~ListAllocator() override;

        void *allocate(uint32 size) override;

        void free(void *pointer) override;

    protected:

        struct MemoryChunk
        {
        public:

            explicit MemoryChunk(uint32 size) : mSize(size) {}

            void remove()
            {
                if (mPrev != nullptr) mPrev->mNext = mNext;
                if (mNext != nullptr) mNext->mPrev = mPrev;
            }

            MemoryChunk* next()
            { return mNext; }

            MemoryChunk* prev()
            { return mPrev; }

            uint32 size()
            { return mSize; }

        protected:

            MemoryChunk* mPrev = nullptr;
            MemoryChunk* mNext = nullptr;
            uint32 mSize = 0;

        };


        struct MemoryBuffer
        {
        public:

            MemoryBuffer(uint32 bufferSize)
            {

            }

            void linkAfter(MemoryBuffer* buffer)
            { mNextBuffer = buffer; }

            MemoryChunk* nextChunk()
            { return mNextChunk; }

            MemoryBuffer* nextBuffer()
            { return mNextBuffer; }

        protected:

            MemoryChunk* mNextChunk = nullptr;
            MemoryBuffer* mNextBuffer = nullptr;

            uint32 mBufferSize = 0;
            uint32 mChunksCount = 0;
            uint32 mMinChunkSize = 0;
            uint32 mMaxChunkSize = 0;

        };

    protected:

    };

} // namespace Berserk

#endif //BERSERK_LISTALLOCATOR_H
