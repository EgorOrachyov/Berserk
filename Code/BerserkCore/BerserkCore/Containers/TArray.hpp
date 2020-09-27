/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TARRAY_HPP
#define BERSERK_TARRAY_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/Platform/Allocator.hpp>
#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/TPredicates.h>
#include <BerserkCore/LogMacro.hpp>
#include <initializer_list>

namespace Berserk {

    template<typename T, typename TAlloc = GlobalAllocator>
    class TArray final {
    public:

        static const uint32 INITIAL_CAPACITY = 2;
        static const uint32 FACTOR = 2;

        explicit TArray(TAlloc alloc = TAlloc()) noexcept : mAlloc(std::move(alloc)) {
            
        }

        TArray(const std::initializer_list<T> &list) : TArray<T>() {
            Add(list);
        }

        TArray(const TArray &other) : mAlloc(other.mAlloc) {
            Add(other);
        }

        TArray(TArray<T> &&other) noexcept
                : mAlloc(std::move(other.mAlloc)),
                  mBuffer(other.mBuffer),
                  mCapacity(other.mCapacity),
                  mSize(other.mSize) {
            other.mBuffer = nullptr;
            other.mSize = 0;
            other.mCapacity = 0;
        }

        ~TArray() {
            if (mBuffer) {
                Clear();
                mAlloc.Free(mBuffer);
                mBuffer = nullptr;
                mCapacity = 0;
                mSize = 0;
            }
        }

        void Resize(uint32 size, const T &e = T()) {
            EnsureCapacity(size);

            if (size < mSize) {
                for (uint32 i = size; i < mSize; i++) {
                    Remove(i);
                }
            } else {
                for (uint32 i = mSize; i < size; i++) {
                    Emplace(e);
                }
            }
        }

        void EnsureCapacity(uint32 desired) {
            if (mCapacity < desired) {
                Expand(desired);
            }
        }

        void EnsureToAdd(uint32 count) {
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

        void Add(const TArray &other) {
            if (this == &other) {
                BERSERK_LOG_ERROR("Containers must differ");
                return;
            }

            EnsureToAdd(other.mSize);

            for (uint32 i = 0; i < other.mSize; i++) {
                new(&mBuffer[mSize]) T(other[i]);
                mSize += 1;
            }
        }

        void Add(const T *buffer, uint32 count) {
            EnsureToAdd(count);

            for (uint32 i = 0; i < count; i++) {
                new(&mBuffer[mSize]) T(buffer[i]);
                mSize += 1;
            }
        }

        T &Move(T &element) {
            return Emplace(std::move(element));
        }

        void Remove(uint32 index) {
            if (index >= mSize) {
                BERSERK_LOG_ERROR("Index out of range");
                return;
            }

            mBuffer[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Memory::Copy(&mBuffer[index], &mBuffer[index + 1], sizeof(T) * (mSize - index));
            }
        }

        void RemoveUnordered(uint32 index) {
            if (index >= mSize) {
                BERSERK_LOG_ERROR("Index out of range")
                return;
            }

            mBuffer[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Memory::Copy(&mBuffer[index], &mBuffer[mSize], sizeof(T));
            }
        }

        template<typename E = TEquals<T>>
        void RemoveElement(const T &toRemove) {
            E equals;
            uint32 i = 0;
            while (i < mSize) {
                if (equals(mBuffer[i], toRemove)) {
                    Remove(i);
                    break;
                }

                i += 1;
            }
        }

        template<typename E = TEquals<T>>
        void RemoveElementUnordered(const T &toRemove) {
            E equals;
            uint32 i = 0;
            while (i < mSize) {
                if (equals(mBuffer[i], toRemove)) {
                    RemoveUnordered(i);
                    break;
                }

                i += 1;
            }
        }

        template<typename E = TEquals <T>>
        bool Contains(const T &element) const {
            E eq;
            for (const auto &e: *this) {
                if (eq(e, element))
                    return true;
            }
            return false;
        }

        TArray &operator=(const std::initializer_list<T> &list) {
            Clear();
            Add(list);
            return *this;
        }

        TArray &operator=(const TArray &other) {
            if (this == &other) {
                return *this;
            }

            Clear();
            Add(other);
            return *this;
        }

        TArray &operator=(TArray &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~TArray();
            new (this) TArray(std::move(other));

            return *this;
        }

        T &operator[](uint32 index) {
            if (index >= mSize) {
                BERSERK_FATAL_ERROR("Index out of range")
            }

            return mBuffer[index];
        }

        const T &operator[](uint32 index) const {
            if (index >= mSize) {
                BERSERK_FATAL_ERROR("Index out of range")
            }

            return mBuffer[index];
        }

        void Clear() {
            for (uint32 i = 0; i < mSize; i++) {
                mBuffer[i].~T();
            }
            mSize = 0;
        }

        /** Clear container and deallocate memory */
        void ClearAndReleaseMemory() {
            if (mBuffer) {
                Clear();
                mAlloc.Free(mBuffer);
                mBuffer = nullptr;
                mCapacity = 0;
                mSize = 0;
            }
        }

        template<typename E = TEquals<T>>
        bool GetIndexOf(const T &element, uint32 &index) const {
            E eq;
            for (uint32 i = 0; i < GetSize(); i++) {
                if (eq(mBuffer[i], element)) {
                    index = i;
                    return true;
                }
            }

            return false;
        }

        bool IsEmpty() const { return mSize == 0; }
        bool IsNotEmpty() const { return mSize > 0; }

        T *GetData() { return mBuffer; }
        const T *GetData() const { return mBuffer; }

        uint32 GetSize() const { return mSize; }
        uint32 GetCapacity() const { return mCapacity; }
        uint32 GetSizeBytes() const { return mSize * sizeof(T); }

        template<typename H = THash<T>>
        uint32 Hash() const {
            H hashGenerator;
            uint64 h = 0;
            for (const auto &a: *this) {
                h += hashGenerator(a);
            }

            return Crc32::Hash(&h, sizeof(h));
        }

        template <typename It>
        class Iterator {
        public:
            Iterator(const TArray<It,TAlloc>* array, uint32 index) {
                mArray = (TArray<It,TAlloc>*) array;
                mIndex = index;
            }

            bool operator==(const Iterator& other) const {
                return (mArray == other.mArray) && ((mArray == nullptr) || (mIndex == other.mIndex));
            }

            bool operator!=(const Iterator& other) const {
                return !(*this == other);
            }

            void operator++() {
                if (mIndex + 1 < mArray->GetSize()) {
                    mIndex += 1;
                }
                else {
                    // If cannot increment - nullify the iterator
                    mArray = nullptr;
                    mIndex = 0;
                }
            }

            void Remove() {
                if (mArray != nullptr && mIndex < mArray->GetSize()) {
                    mArray->Remove(mIndex);

                    // If after index no more elements - nullify the iterator
                    if (mIndex >= mArray->GetSize()) {
                        mArray = nullptr;
                        mIndex = 0;
                    }
                }
            }

            It& operator*() {
                return (*mArray)[mIndex];
            }

        private:
            TArray<It,TAlloc>* mArray;
            uint32 mIndex = 0;
        };

        Iterator<const T> begin() const { return Iterator<const T>(this, 0); }
        Iterator<T> begin() { return Iterator<T>(this, 0); }

        Iterator<const T> end() const { return Iterator<T>(nullptr, 0); }
        Iterator<T> end() { return Iterator<T>(nullptr, 0); }

    private:

        void Expand() {
            if (mCapacity == 0) {
                mCapacity = INITIAL_CAPACITY;
                mBuffer = (T *) mAlloc.Allocate(mCapacity * sizeof(T));
            } else {
                auto newCapacity = mCapacity * FACTOR;
                auto newBuffer = (T *) mAlloc.Allocate(newCapacity * sizeof(T));
                Memory::Copy(newBuffer, mBuffer, mSize * sizeof(T));
                mAlloc.Free(mBuffer);
                mBuffer = newBuffer;
                mCapacity = newCapacity;
            }
        }

        void Expand(uint32 capacity) {
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
                mAlloc.Free(mBuffer);
                mBuffer = newBuffer;
                mCapacity = newCapacity;
            }
        }

    private:
        TAlloc mAlloc;
        T *mBuffer = nullptr;
        uint32 mCapacity = 0;
        uint32 mSize = 0;
    };

}

#endif //BERSERK_TARRAY_HPP