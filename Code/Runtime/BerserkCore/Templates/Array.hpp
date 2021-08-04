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

#ifndef BERSERK_ARRAY_HPP
#define BERSERK_ARRAY_HPP

#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Templates/Contracts.hpp>
#include <BerserkCore/Platform/Crc32.hpp>
#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/Platform/Allocator.hpp>
#include <initializer_list>

namespace Berserk {

    /**
     * @brief Dynamic array
     *
     * Dynamically resizable array for typed T elements.
     * Stores elements in place in the allocated by Alloc buffer.
     *
     * @tparam T Type of stored elements
     * @tparam Alloc Provided allocator for dynamic allocations
     */
    template<typename T, typename Alloc = Allocator>
    class Array {
    public:

        static const uint64 INITIAL_CAPACITY = 2;
        static const uint64 FACTOR = 2;

        explicit Array(Alloc&& alloc = Alloc())
            : mAlloc(std::move(alloc)) {

        }

        Array(const std::initializer_list<T> &list) : Array<T>() {
            Add(list);
        }

        Array(const Array &other) : mAlloc(other.mAlloc) {
            Add(other);
        }

        Array(Array &&other) noexcept
                : mAlloc(std::move(other.mAlloc)), mBuffer(other.mBuffer), mCapacity(other.mCapacity),
                  mSize(other.mSize) {
            other.mBuffer = nullptr;
            other.mSize = 0;
            other.mCapacity = 0;
        }

        ~Array() {
            if (mBuffer) {
                Clear();
                mAlloc.Deallocate(mBuffer);
                mBuffer = nullptr;
                mCapacity = 0;
                mSize = 0;
            }
        }

        void Resize(uint64 size, const T &e = T()) {
            EnsureCapacity(size);

            if (size < mSize) {
                while (size < mSize) {
                    Remove(size);
                }
            } else {
                for (uint64 i = mSize; i < size; i++) {
                    Emplace(e);
                }
            }
        }

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

        template<typename ... TArgs>
        T &Emplace(TArgs &&... args) {
            if (mSize >= mCapacity) {
                Expand();
            }

            T *obj = new(&mBuffer[mSize]) T(std::forward<TArgs>(args)...);
            mSize += 1;

            return *obj;
        }

        T &Add(const T &element) {
            if (mSize >= mCapacity) {
                Expand();
            }

            T *obj = new(&mBuffer[mSize]) T(element);
            mSize += 1;

            return *obj;
        }

        void Add(const std::initializer_list<T> &list) {
            EnsureToAdd(list.size());

            for (const auto &e: list) {
                new(&mBuffer[mSize]) T(e);
                mSize += 1;
            }
        }

        void Add(const Array &other) {
            assert(this != &other);

            EnsureToAdd(other.mSize);

            for (uint64 i = 0; i < other.mSize; i++) {
                new(&mBuffer[mSize]) T(other[i]);
                mSize += 1;
            }
        }

        void Add(const T *buffer, uint64 count) {
            EnsureToAdd(count);

            for (uint64 i = 0; i < count; i++) {
                new(&mBuffer[mSize]) T(buffer[i]);
                mSize += 1;
            }
        }

        T &Move(T &&element) {
            return Emplace(std::move(element));
        }

        T PopLast() {
            assert(GetSize() > 0);

            T result = std::move(mBuffer[GetSize() - 1]);
            mSize -= 1;

            return result;
        }

        void Remove(uint64 index) {
            assert(index < GetSize());

            mBuffer[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Memory::Copy(&mBuffer[index], &mBuffer[index + 1], sizeof(T) * (mSize - index));
            }
        }

        void RemoveUnordered(uint64 index) {
            assert(index < GetSize());

            mBuffer[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Memory::Copy(&mBuffer[index], &mBuffer[mSize], sizeof(T));
            }
        }

        template<typename E = Equals<T>>
        void RemoveElement(const T &toRemove, E&& equals = E()) {
            uint32 i = 0;
            while (i < mSize) {
                if (equals(GetData()[i], toRemove)) {
                    Remove(i);
                    break;
                }

                i += 1;
            }
        }

        template<typename E = Equals <T>>
        void RemoveElementUnordered(const T &toRemove, E&& equals = E()) {
            uint32 i = 0;
            while (i < mSize) {
                if (equals(GetData()[i], toRemove)) {
                    RemoveUnordered(i);
                    break;
                }

                i += 1;
            }
        }

        template<typename E = Equals<T>>
        bool Contains(const T &element) const {
            E eq;
            for (const auto &e: *this) {
                if (eq(e, element))
                    return true;
            }
            return false;
        }

        Array &operator=(const std::initializer_list<T> &list) {
            Clear();
            Add(list);
            return *this;
        }

        Array &operator=(const Array &other) {
            if (this == &other) {
                return *this;
            }

            Clear();
            Add(other);
            return *this;
        }

        Array &operator=(Array &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~Array();
            new(this) Array(std::move(other));

            return *this;
        }

        T &operator[](uint64 index) {
            assert(index < GetSize());

            return mBuffer[index];
        }

        const T &operator[](uint64 index) const {
            assert(index < GetSize());

            return mBuffer[index];
        }

        T& Last() {
            assert(IsNotEmpty());
            return mBuffer[GetSize() - 1];
        }

        const T& Last() const {
            assert(IsNotEmpty());
            return mBuffer[GetSize() - 1];
        }

        void Clear() {
            for (uint64 i = 0; i < mSize; i++) {
                mBuffer[i].~T();
            }
            mSize = 0;
        }

        /** Clear container and deallocate memory */
        void ClearAndReleaseMemory() {
            if (mBuffer) {
                Clear();
                mAlloc.Deallocate(mBuffer);
                mBuffer = nullptr;
                mCapacity = 0;
                mSize = 0;
            }
        }

        template<typename Predicate>
        bool FindFirst(uint64 &index, Predicate&& predicate = Predicate()) const {
            for (uint64 i = 0; i < GetSize(); i++) {
                if (predicate(mBuffer[i])) {
                    index = i;
                    return true;
                }
            }

            return false;
        }

        template<typename E = Equals<T>>
        bool Equals(const Array &other, E&& equals = E()) {
            if (this == &other) {
                return true;
            }

            if (GetSize() != other.GetSize()) {
                return false;
            }

            for (uint64 i = 0; i < GetSize(); i++) {
                if (!equals(mBuffer[i], other.mBuffer[i])) {
                    return false;
                }
            }

            return true;
        }

        bool IsEmpty() const {
            return mSize == 0;
        }

        bool IsNotEmpty() const {
            return mSize > 0;
        }

        T *GetData() {
            return mBuffer;
        }

        const T *GetData() const {
            return mBuffer;
        }

        uint64 GetSize() const {
            return mSize;
        }

        uint64 GetCapacity() const {
            return mCapacity;
        }

        uint64 GetSizeBytes() const {
            return mSize * sizeof(T);
        }

        Alloc& GetAlloc() {
            return mAlloc;
        }

        template<typename H = Hash<T>>
        uint32 Hash() const {
            H hashGenerator;
            uint64 h = 0;

            // Iterate over container and hash all values
            for (const auto &a: *this) {
                h += (uint64) hashGenerator(a);
            }

            return Crc32::Hash(&h, sizeof(h));
        }

        template<typename TElement>
        class IteratorBase {
        protected:
            friend class Array<T, Alloc>;

            IteratorBase(TElement *buffer, uint64 index, uint64 size)
                    : mBuffer(buffer), mIndex(index), mSize(size) {

            }

        public:

            bool operator==(const IteratorBase &other) const {
                return mBuffer == other.mBuffer && mIndex == other.mIndex;
            }

            bool operator!=(const IteratorBase &other) const {
                return !(*this == other);
            }

            void operator++() {
                if (mIndex + 1 < mSize) {
                    mIndex += 1;
                    return;
                }

                if (mBuffer != nullptr) {
                    mIndex = 0;
                    mSize = 0;
                    mBuffer = nullptr;
                }
            }

            TElement &operator*() {
                return mBuffer[mIndex];
            }

        protected:
            TElement *mBuffer;
            uint64 mIndex = 0;
            uint64 mSize = 0;
        };

        IteratorBase<T> Remove(const IteratorBase<T> element) {
            assert(element != end());
            assert(element.mBuffer == mBuffer);
            assert(element.mSize == mSize);
            assert(element.mIndex < mSize);

            Remove(element.mIndex);

            return {IsNotEmpty() ? GetData() : nullptr, element.mIndex, GetSize()};
        }

        IteratorBase<T> RemoveUnordered(const IteratorBase<T> element) {
            assert(element != end());
            assert(element.mBuffer == mBuffer);
            assert(element.mSize == mSize);
            assert(element.mIndex < mSize);

            RemoveUnordered(element.mIndex);

            return {IsNotEmpty() ? GetData() : nullptr, element.mIndex, GetSize()};
        }

        IteratorBase<T> begin() {
            return {IsNotEmpty() ? GetData() : nullptr, 0, GetSize()};
        }

        IteratorBase<T> end() {
            return {nullptr, 0, 0};
        }

        IteratorBase<const T> begin() const {
            return {IsNotEmpty() ? GetData() : nullptr, 0, GetSize()};
        }

        IteratorBase<const T> end() const {
            return {nullptr, 0, 0};
        }

    private:

        void Expand() {
            if (mCapacity == 0) {
                mCapacity = INITIAL_CAPACITY;
                mBuffer = (T *) mAlloc.Allocate(mCapacity * sizeof(T));
            } else {
                auto newCapacity = mCapacity * FACTOR;
                auto newBuffer = (T *) mAlloc.Allocate(newCapacity * sizeof(T));
                Memory::Copy(newBuffer, mBuffer, mSize * sizeof(T));
                mAlloc.Deallocate(mBuffer);
                mBuffer = newBuffer;
                mCapacity = newCapacity;
            }
        }

        void Expand(uint64 capacity) {
            if (mCapacity == 0) {
                mCapacity = INITIAL_CAPACITY;
                // Expand accordingly to size factor to avoid unintended small allocations
                while (mCapacity < capacity) {
                    mCapacity *= FACTOR;
                }

                mBuffer = (T *) mAlloc.Allocate(mCapacity * sizeof(T));
            } else {
                auto newCapacity = mCapacity;
                // Expand accordingly to size factor to avoid unintended small allocations
                while (newCapacity < capacity) {
                    newCapacity *= FACTOR;
                }

                auto newBuffer = (T *) mAlloc.Allocate(newCapacity * sizeof(T));
                Memory::Copy(newBuffer, mBuffer, mSize * sizeof(T));
                mAlloc.Deallocate(mBuffer);
                mBuffer = newBuffer;
                mCapacity = newCapacity;
            }
        }

    private:
        Alloc mAlloc;
        T *mBuffer = nullptr;
        uint64 mCapacity = 0;
        uint64 mSize = 0;
    };

    /**
     * Implement contracts for the array
     */
    template<typename T, typename Alloc>
    class Equals<Array<T, Alloc>> {
    public:
        using Type = Array<T, Alloc>;

        bool operator()(const Type &a, const Type &b) const {
            return a.Equals(b);
        }

    };

    template<typename T, typename Alloc>
    class Hash<Array<T, Alloc>> {
    public:
        using Type = Array<T, Alloc>;

        uint32 operator()(const Type &a) const {
            return a.Hash();
        }

    };

    template <typename T, typename Alloc>
    class TextPrint<Array<T, Alloc>> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const Array<T,Alloc> &array) const {
            TextPrint<T> printT;

            stream.Add(BERSERK_TEXT("["));

            auto size = array.GetSize();
            if (size > 0) {
                printT(stream, array[0]);

                for (uint64 i = 1; i < size; i++) {
                    stream.Add(BERSERK_TEXT(", "));
                    printT(stream, array[i]);
                }
            }

            stream.Add(BERSERK_TEXT("]"));
        }
    };
}

#endif //BERSERK_ARRAY_HPP