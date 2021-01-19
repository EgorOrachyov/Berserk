/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_ARRAY_HPP
#define BERSERK_ARRAY_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Misc/Contracts.hpp>
#include <BerserkCore/Misc/Crc32.hpp>
#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/Platform/Allocator.hpp>
#include <BerserkCore/Assert.hpp>
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
    template<typename T, typename Alloc = Platform::Allocator>
    class Array {
    public:

        static const size_t INITIAL_CAPACITY = 2;
        static const size_t FACTOR = 2;

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

        void Resize(size_t size, const T &e = T()) {
            EnsureCapacity(size);

            if (size < mSize) {
                for (size_t i = size; i < mSize; i++) {
                    Remove(i);
                }
            } else {
                for (size_t i = mSize; i < size; i++) {
                    Emplace(e);
                }
            }
        }

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
            BERSERK_ASSERT(this != &other);

            EnsureToAdd(other.mSize);

            for (size_t i = 0; i < other.mSize; i++) {
                new(&mBuffer[mSize]) T(other[i]);
                mSize += 1;
            }
        }

        void Add(const T *buffer, size_t count) {
            EnsureToAdd(count);

            for (size_t i = 0; i < count; i++) {
                new(&mBuffer[mSize]) T(buffer[i]);
                mSize += 1;
            }
        }

        T &Move(T &element) {
            return Emplace(std::move(element));
        }

        void Remove(size_t index) {
            BERSERK_ASSERT(index < GetSize());

            mBuffer[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Platform::Memory::Copy(&mBuffer[index], &mBuffer[index + 1], sizeof(T) * (mSize - index));
            }
        }

        void RemoveUnordered(size_t index) {
            BERSERK_ASSERT(index < GetSize());

            mBuffer[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Platform::Memory::Copy(&mBuffer[index], &mBuffer[mSize], sizeof(T));
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

        T &operator[](size_t index) {
            BERSERK_ASSERT(index < GetSize());

            return mBuffer[index];
        }

        const T &operator[](size_t index) const {
            BERSERK_ASSERT(index < GetSize());

            return mBuffer[index];
        }

        void Clear() {
            for (size_t i = 0; i < mSize; i++) {
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

        template<typename E = Equals<T>>
        bool GetIndexOf(const T &element, size_t &index, E&& equals = E()) const {
            for (size_t i = 0; i < GetSize(); i++) {
                if (equals(mBuffer[i], element)) {
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

            for (size_t i = 0; i < GetSize(); i++) {
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

        size_t GetSize() const {
            return mSize;
        }

        size_t GetCapacity() const {
            return mCapacity;
        }

        size_t GetSizeBytes() const {
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

            IteratorBase(TElement *buffer, size_t index, size_t size)
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
            size_t mIndex = 0;
            size_t mSize = 0;
        };

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
                Platform::Memory::Copy(newBuffer, mBuffer, mSize * sizeof(T));
                mAlloc.Deallocate(mBuffer);
                mBuffer = newBuffer;
                mCapacity = newCapacity;
            }
        }

        void Expand(size_t capacity) {
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
                Platform::Memory::Copy(newBuffer, mBuffer, mSize * sizeof(T));
                mAlloc.Deallocate(mBuffer);
                mBuffer = newBuffer;
                mCapacity = newCapacity;
            }
        }

    private:
        Alloc mAlloc;
        T *mBuffer = nullptr;
        size_t mCapacity = 0;
        size_t mSize = 0;
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

                for (size_t i = 1; i < size; i++) {
                    stream.Add(BERSERK_TEXT(", "));
                    printT(stream, array[i]);
                }
            }

            stream.Add(BERSERK_TEXT("]"));
        }
    };
}

#endif //BERSERK_ARRAY_HPP