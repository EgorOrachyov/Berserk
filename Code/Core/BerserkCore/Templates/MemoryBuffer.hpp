/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_MEMORYBUFFER_HPP
#define BERSERK_MEMORYBUFFER_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Platform/Allocator.hpp>
#include <BerserkCore/Templates/RefCounted.hpp>

namespace Berserk {

    /** Read-only ref counted memory buffer for safe data passing  */
    class ReadOnlyMemoryBuffer: public RefCountedThreadSafe {
    public:
        ~ReadOnlyMemoryBuffer() override = default;

        /** @return Pointer to the read-only memory with data  */
        virtual const void* GetData() const = 0;

        /** @return Size of the memory data */
        virtual size_t GetSize() const = 0;
    };

    /** Ref counted memory buffer for safe data passing with mutable data storage  */
    class MemoryBuffer: public ReadOnlyMemoryBuffer {
    public:
        ~MemoryBuffer() override = default;

        /** @return Pointer to the memory with data  */
        virtual void* GetData() = 0;
    };

    /**
     * Memory buffer with customizable templated allocator for memory region.
     * Actual memory object is allocated with default system allocator.
     *
     * @tparam Alloc Type of the allocator for buffer allocation
     */
    template<typename Alloc = Allocator>
    class MemoryBufferGeneric: public MemoryBuffer {
    public:

        ~MemoryBufferGeneric() override {
            if (mPtr) {
                mAlloc.Deallocate(mPtr);
                mPtr = nullptr;
                mSize = 0;
            }
        }

        /**
         * Resize this buffer to a new size.
         * Previous memory region is deallocated and data is invalidated.
         *
         * @param newSize New buffer size in bytes
         */
        void Resize(size_t newSize) {
            if (mPtr) {
                mAlloc.Deallocate(mPtr);
                mPtr = nullptr;
            }

            mSize = newSize;

            if (newSize > 0)
                mPtr = mAlloc.Allocate(newSize);
        }

        void* GetData() override {
            return mPtr;
        }

        const void *GetData() const override {
            return mPtr;
        }

        size_t GetSize() const override {
            return mSize;
        }

        /**
         * Create memory buffer with optionally passed allocator.
         * @param alloc Allocator for buffer memory allocation.
         *
         * @return Reference counted pointer to created memory buffer object.
         */
        static RefCounted<MemoryBufferGeneric<Alloc>> Create(Alloc&& alloc = Alloc()) {
            auto buffer = Memory::Make<MemoryBufferGeneric<Alloc>>(std::move(alloc));
            return RefCounted<MemoryBufferGeneric<Alloc>>(buffer);
        }

        /**
         * Create memory buffer of specified size with optionally passed allocator and fill with data.
         *
         * @param byteSize Size of buffer to create
         * @param data Optional data to copy (of size `byteSize`). If null then not copied.
         * @param alloc Allocator for buffer memory allocation.
         *
         * @return Reference counted pointer to created memory buffer object.
         */
        static RefCounted<MemoryBufferGeneric<Alloc>> Create(size_t byteSize, const void* data, Alloc&& alloc = Alloc()) {
            auto buffer = Create(std::move(alloc));

            buffer->Resize(byteSize);

            if (data)
                Memory::Copy(buffer->GetData(), data, byteSize);

            return buffer;
        }

    protected:

        friend class Memory;

        // Can create buffer only by public static utils functions
        explicit MemoryBufferGeneric(Alloc alloc = Alloc())
            : mAlloc(std::move(alloc)) {
        }

        void OnReleased() const override {
            Memory::Release(this);
        }

    private:
        Alloc mAlloc;
        void* mPtr = nullptr;
        size_t mSize = 0;
    };

    /** Memory buffer with default system malloc */
    using SystemMemoryBuffer = MemoryBufferGeneric<Allocator>;

}

#endif //BERSERK_MEMORYBUFFER_HPP