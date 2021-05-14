/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
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

        static const size_t INITIAL_CAPACITY = 2;
        static const size_t FACTOR = 2;

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

        void EnsureCapacity(size_t desired) {
            if (mCapacity < desired) {
                Expand(desired);
            }
        }

        void EnsureToAdd(size_t count) {
            if (mCapacity < (mSize + count)) {
                Expand(mSize + count);
            }
        }

        void Clear() {
            for (size_t i = mHead; i < mSize; i++) {
                size_t index = i % mCapacity;
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

            for (size_t i = mHead; i < mSize; i++) {
                size_t index = i % mCapacity;
                array.Add(mBuffer[index]);
            }
        }

        void PopValues(Array<T>& array) {
            array.EnsureToAdd(GetSize());

            for (size_t i = mHead; i < mSize; i++) {
                size_t index = i % mCapacity;

                array.Move(std::move(mBuffer[index]));
                mBuffer[index].~T();
            }

            mHead = mTail = mSize = 0;
        }

        size_t GetSize() const {
            return mSize;
        }

        size_t GetCapacity() const {
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
                        size_t part = mTail - mHead;
                        Memory::Copy(&newBuffer[0], &mBuffer[mHead], sizeof(T) * part);
                    }
                    else {
                        size_t firstPart = mCapacity - mHead;
                        size_t secondPart = mTail;

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

        void Expand(size_t capacity) {
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
                        size_t part = mTail - mHead;
                        Memory::Copy(&newBuffer[0], &mBuffer[mHead], sizeof(T) * part);
                    }
                    else {
                        size_t firstPart = mCapacity - mHead;
                        size_t secondPart = mTail;

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
        size_t mCapacity = 0;
        size_t mSize = 0;
        size_t mHead = 0;
        size_t mTail = 0;
    };

}

#endif //BERSERK_QUEUE_HPP