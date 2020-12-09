/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_MEMORYBUFFER_HPP
#define BERSERK_MEMORYBUFFER_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/Platform/Allocator.hpp>

namespace Berserk {
    namespace Misc {

        template <typename Alloc = Platform::Allocator>
        class MemoryBuffer {
        public:

            explicit MemoryBuffer(Alloc allocator = Alloc())
                : mAllocator(std::move(allocator)) {

            }

            explicit MemoryBuffer(size_t size) {
                mSize = size;
                mBuffer = mAllocator.Allocate(size);
            }

            MemoryBuffer(size_t size, const void* data)
                : MemoryBuffer(size) {
                if (data != nullptr) {
                    Platform::Memory::Copy(mBuffer, data, size);
                }
            }

            MemoryBuffer(const MemoryBuffer& other)
                : MemoryBuffer(other.GetSize(), other.GetBuffer()) {

            }

            MemoryBuffer(MemoryBuffer&& other) noexcept
                : mAllocator(std::move(other.mAllocator)), mSize(other.GetSize()), mBuffer(other.GetBuffer()) {
                other.mSize = 0;
                other.mBuffer = nullptr;
            }

            ~MemoryBuffer() {
                if (!IsEmpty()) {
                    mAllocator.Deallocate(mBuffer);
                    mSize = 0;
                    mBuffer = nullptr;
                }
            }

            MemoryBuffer& operator=(const MemoryBuffer& other) {
                if (this == &other) {
                    return *this;
                }

                this->~MemoryBuffer();
                new (this) MemoryBuffer(other);
                return *this;
            }

            MemoryBuffer& operator=(MemoryBuffer&& other) noexcept {
                if (this == &other) {
                    return *this;
                }

                this->~MemoryBuffer();
                new (this) MemoryBuffer(std::move(other));
                return *this;
            }

            bool IsEmpty() const {
                return mSize == 0;
            }

            size_t GetSize() const {
                return mSize;
            }

            void* GetBuffer() const {
                return mBuffer;
            }

        private:
            Alloc mAllocator;
            size_t mSize = 0;
            void* mBuffer = nullptr;
        };

    }
}

#endif //BERSERK_MEMORYBUFFER_HPP