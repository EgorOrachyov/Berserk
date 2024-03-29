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

#ifndef BERSERK_QUEUE_HPP
#define BERSERK_QUEUE_HPP

#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Math/Utils.hpp>
#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Platform/Allocator.hpp>

namespace Berserk {

    /**
     * @brief Dynamic queue
     *
     * Dynamically resizable queue for typed T elements.
     * Stores elements in place, requires elements to be movable in memory.
     *
     * @tparam T Type of stored elements
     * @tparam Alloc Type of used allocator for memory operations
     */
    template <typename T, typename Alloc = Allocator>
    class Queue {
    public:

        static const uint64 INITIAL_CAPACITY = 2;
        static const uint64 FACTOR = 2;

        explicit Queue(Alloc&& alloc = Alloc())
            : mAlloc(std::move(alloc)) {

        }

        Queue(const std::initializer_list<T> &list): Queue() {
            PushAll(list);
        }

        Queue(const Queue& other)
            : mAlloc(other.mAlloc) {
            PushAll(other);
        }

        Queue(Queue&& other) noexcept
            : mAlloc(std::move(other.mAlloc)) {
            mBuffer = other.mBuffer;
            mSize = other.mSize;
            mCapacity = other.mCapacity;
            mHead = other.mHead;
            mTail = other.mTail;

            other.mBuffer = nullptr;
            other.mHead = 0;
            other.mTail = 0;
            other.mSize = 0;
            other.mCapacity = 0;
        }

        ~Queue() {
            if (mBuffer) {
                Clear();
                mAlloc.Deallocate(mBuffer);
                mBuffer = nullptr;
                mCapacity = 0;
                mSize = 0;
                mHead = 0;
                mTail = 0;
            }
        }

        Queue& operator=(const Queue& other) {
            if (this == &other) {
                return *this;
            }

            this->~Queue();
            new (this) Queue(other);
            return *this;
        }

        Queue& operator=(Queue&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~Queue();
            new (this) Queue(std::move(other));
            return *this;
        };

        void EnsureCapacity(uint64 desired) {
            if (mCapacity < desired) {
                Expand(desired);
            }
        }

        void EnsureToAdd(uint64 count) {
            if (mCapacity < (mSize + count)) {
                Expand(mSize + count);
            }
        }

        void Clear() {
            for (uint64 i = 0; i < mSize; i++) {
                uint64 index = (mHead + i) % mCapacity;
                mBuffer[index].~T();
            }
            mHead = mTail = mSize = 0;
        }

        T& Push(const T& element) {
            ExpandIfNeeded();

            auto e = new (&mBuffer[mTail]) T(element);
            mTail = (mTail + 1) % mCapacity;
            mSize += 1;
            return *e;
        }

        T& PushMove(T& element) {
            ExpandIfNeeded();

            auto e = new (&mBuffer[mTail]) T(std::move(element));
            mTail = (mTail + 1) % mCapacity;
            mSize += 1;
            return *e;
        }

        void PushAll(const std::initializer_list<T>& container) {
            EnsureToAdd(container.size());

            for (const auto& e: container) {
                Push(e);
            }
        }

        template<typename Container>
        void PushAll(const Container& container) {
            EnsureToAdd(container.GetSize());

            for (const auto& e: container) {
                Push(e);
            }
        }

        bool Pop(T& element) {
            if (IsNotEmpty()) {
                mSize -= 1;
                element = std::move(mBuffer[mHead]);
                mBuffer[mHead].~T();
                mHead = (mHead + 1) % mCapacity;
                return true;
            }

            return false;
        }

        bool Peek(T& element) {
            if (IsNotEmpty()) {
                element = mBuffer[mHead];
                return true;
            }

            return false;
        }

        void PeekValues(Array<T>& array) const {
            array.EnsureToAdd(GetSize());

            for (uint64 i = 0; i < mSize; i++) {
                uint64 index = (mHead + i) % mCapacity;
                array.Add(mBuffer[index]);
            }
        }

        void PopValues(Array<T>& array) {
            array.EnsureToAdd(GetSize());

            for (uint64 i = 0; i < mSize; i++) {
                uint64 index = (mHead + i) % mCapacity;

                array.Move(std::move(mBuffer[index]));
                mBuffer[index].~T();
            }

            mHead = mTail = mSize = 0;
        }

        uint64 GetSize() const {
            return mSize;
        }

        uint64 GetCapacity() const {
            return mCapacity;
        }

        bool IsEmpty() const {
            return mSize == 0;
        }

        bool IsNotEmpty() const {
            return mSize > 0;
        }

    private:

        void Expand() {
            if (mCapacity == 0) {
                mCapacity = INITIAL_CAPACITY;
                mBuffer = (T*) mAlloc.Allocate(sizeof(T) * mCapacity);
            }
            else {
                auto newCapacity = mCapacity * FACTOR;
                auto newBuffer = (T *) mAlloc.Allocate(newCapacity * sizeof(T));

                if (mSize > 0) {
                    if (mHead < mTail) {
                        uint64 part = mTail - mHead;
                        Memory::Copy(&newBuffer[0], &mBuffer[mHead], sizeof(T) * part);
                    }
                    else {
                        uint64 firstPart = mCapacity - mHead;
                        uint64 secondPart = mTail;

                        Memory::Copy(&newBuffer[0], &mBuffer[mHead], sizeof(T) * firstPart);
                        Memory::Copy(&newBuffer[firstPart], &mBuffer[0], sizeof(T) * secondPart);
                    }
                }

                mAlloc.Deallocate(mBuffer);
                mBuffer = newBuffer;
                mCapacity = newCapacity;
                mHead = 0;
                mTail = mSize;
            }
        }

        void Expand(uint64 capacity) {
            if (mCapacity == 0) {
                mCapacity = INITIAL_CAPACITY;

                // Expand accordingly to size factor to avoid unintended small allocations
                while (mCapacity < capacity) {
                    mCapacity *= FACTOR;
                }

                mBuffer = (T*) mAlloc.Allocate(sizeof(T) * mCapacity);
            }
            else {
                auto newCapacity = mCapacity * FACTOR;

                // Expand accordingly to size factor to avoid unintended small allocations
                while (mCapacity < capacity) {
                    mCapacity *= FACTOR;
                }

                auto newBuffer = (T *) mAlloc.Allocate(newCapacity * sizeof(T));

                if (mSize > 0) {
                    if (mHead < mTail) {
                        uint64 part = mTail - mHead;
                        Memory::Copy(&newBuffer[0], &mBuffer[mHead], sizeof(T) * part);
                    }
                    else {
                        uint64 firstPart = mCapacity - mHead;
                        uint64 secondPart = mTail;

                        Memory::Copy(&newBuffer[0], &mBuffer[mHead], sizeof(T) * firstPart);
                        Memory::Copy(&newBuffer[firstPart], &mBuffer[0], sizeof(T) * secondPart);
                    }
                }

                mAlloc.Deallocate(mBuffer);
                mBuffer = newBuffer;
                mCapacity = newCapacity;
                mHead = 0;
                mTail = mSize;
            }
        }

        void ExpandIfNeeded() {
            if (mSize == mCapacity || mCapacity == 0) {
                Expand();
            }
        }

        Alloc mAlloc;
        T* mBuffer = nullptr;
        uint64 mCapacity = 0;
        uint64 mSize = 0;
        uint64 mHead = 0;
        uint64 mTail = 0;
    };

}

#endif //BERSERK_QUEUE_HPP