/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_LINEARALLOCATOR_HPP
#define BERSERK_LINEARALLOCATOR_HPP

#include <BerserkCore/Assert.hpp>
#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Platform/Allocator.hpp>

namespace Berserk {

    /**
     * @brief Linear Allocator
     *
     * Allocates memory sequentially in the linear fixed size chunk of the pre-allocated memory.
     * Allows to free all the used memory at once.
     *
     * @tparam Alloc Template allocator type, used to allocate fixed memory buffer for this allocator.
     */
    template <typename Alloc = Allocator>
    class LinearAllocator {
    public:
        using AllocatorType = Alloc;
        static const size_t DEFAULT_SIZE = Memory::KiB;
        static const size_t DEFAULT_ALIGNMENT = Memory::ALIGNMENT;

        explicit LinearAllocator(size_t bufferSize = DEFAULT_SIZE, size_t alignment = DEFAULT_ALIGNMENT, Alloc&& alloc = Alloc())
            : mAlloc(std::move(alloc)),
              mBufferSize(bufferSize),
              mAlignment(alignment) {

            BERSERK_ASSERT(mBufferSize > 0);
            BERSERK_ASSERT(mAlignment > 0);

            mBuffer = (uint8*) mAlloc.Allocate(mBufferSize);

            BERSERK_ASSERT(mBuffer);
        }

        LinearAllocator(LinearAllocator&& other) noexcept
           : mAlloc(std::move(other.mAlloc)),
             mBufferSize(other.mBufferSize),
             mAlignment(other.mAlignment),
             mAllocated(other.mAllocated),
             mBuffer(other.mBuffer) {

            other.mBuffer = nullptr;
            other.mBufferSize = 0;
            other.mAllocated = 0;
        }

        ~LinearAllocator() {
            if (mBuffer) {
                mAlloc.Deallocate(mBuffer);
                mBuffer = nullptr;
                mBufferSize = 0;
                mAllocated = 0;
            }
        }

        void* Allocate(size_t bytesToAllocate) {
            size_t alignedSize = Memory::AlignSize(bytesToAllocate, mAlignment);

            BERSERK_ASSERT(mAllocated + alignedSize <= mBufferSize);

            if (mAllocated + alignedSize <= mBufferSize) {
                uint8* ptr = &mBuffer[mAllocated];
                mAllocated += alignedSize;
                return ptr;
            }

            return nullptr;
        }

        void Deallocate(void* memory) {
            // empty, all allocated memory is freed at once
        }

        void Reset() {
            mAllocated = 0;
        }

        size_t GetAllocatedSize() const {
            return mAllocated;
        }

        size_t GetBufferSize() const {
            return mBufferSize;
        }

        size_t GetAlignment() const {
            return mAlignment;
        }

        Alloc& GetAlloc() {
            return mAlloc;
        }

    private:
        Alloc mAlloc;
        size_t mBufferSize;
        size_t mAlignment;
        size_t mAllocated = 0;
        uint8* mBuffer = nullptr;
    };

}

#endif //BERSERK_LINEARALLOCATOR_HPP