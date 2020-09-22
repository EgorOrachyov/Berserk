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

        explicit TArray(TAlloc alloc = TAlloc()) noexcept : mAlloc(std::move(alloc)) {}

        TArray(const std::initializer_list<T> &list) : TArray<T>() {
            add(list);
        }

        TArray(const TArray &other) : TArray<T>() {
            mAlloc = other.mAlloc;
            add(other);
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
                clear();
                mAlloc.free(mBuffer);
                mBuffer = nullptr;
                mCapacity = 0;
                mSize = 0;
            }
        }

        void resize(uint32 size, const T &e = T()) {
            ensureCapacity(size);

            if (size < mSize) {
                for (uint32 i = size; i < mSize; i++) {
                    remove(i);
                }
            } else {
                for (uint32 i = mSize; i < size; i++) {
                    emplace(e);
                }
            }
        }

        void ensureCapacity(uint32 desired) {
            if (mCapacity < desired) {
                expand(desired);
            }
        }

        void ensureToAdd(uint32 count) {
            if (mCapacity < (mSize + count)) {
                expand(mSize + count);
            }
        }

        template<typename ... TArgs>
        T &emplace(TArgs &&... args) {
            if (mSize >= mCapacity) {
                expand();
            }

            T *obj = new(&mBuffer[mSize]) T(std::forward<TArgs>(args)...);
            mSize += 1;

            return *obj;
        }

        T &add(const T &element) {
            if (mSize >= mCapacity) {
                expand();
            }

            T *obj = new(&mBuffer[mSize]) T(element);
            mSize += 1;

            return *obj;
        }

        void add(const std::initializer_list<T> &list) {
            ensureToAdd(list.size());

            for (const auto &e: list) {
                new(&mBuffer[mSize]) T(e);
                mSize += 1;
            }
        }

        void add(const TArray &other) {
            if (this == &other) {
                BERSERK_LOG_ERROR("Containers must differ");
                return;
            }

            ensureToAdd(other.mSize);

            for (uint32 i = 0; i < other.mSize; i++) {
                new(&mBuffer[mSize]) T(other[i]);
                mSize += 1;
            }
        }

        void add(const T *buffer, uint32 count) {
            ensureToAdd(count);

            for (uint32 i = 0; i < count; i++) {
                new(&mBuffer[mSize]) T(buffer[i]);
                mSize += 1;
            }
        }

        T &move(T &element) {
            return emplace(std::move(element));
        }

        void remove(uint32 index) {
            if (index >= mSize) {
                BERSERK_LOG_ERROR("Index out of range");
                return;
            }

            mBuffer[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Memory::copy(&mBuffer[index], &mBuffer[index + 1], sizeof(T) * (mSize - index));
            }
        }

        void removeUnordered(uint32 index) {
            if (index >= mSize) {
                BERSERK_LOG_ERROR("Index out of range")
                return;
            }

            mBuffer[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Memory::copy(&mBuffer[index], &mBuffer[mSize], sizeof(T));
            }
        }

        template<typename E = TEquals<T>>
        void removeElement(const T &toRemove) {
            E equals;
            uint32 i = 0;
            while (i < mSize) {
                if (equals(mBuffer[i], toRemove)) {
                    remove(i);
                    break;
                }

                i += 1;
            }
        }

        template<typename E = TEquals<T>>
        void removeElementUnordered(const T &toRemove) {
            E equals;
            uint32 i = 0;
            while (i < mSize) {
                if (equals(mBuffer[i], toRemove)) {
                    removeUnordered(i);
                    break;
                }

                i += 1;
            }
        }

        template<typename E = TEquals <T>>
        bool contains(const T &element) const {
            E eq;
            for (const auto &e: *this) {
                if (eq(e, element))
                    return true;
            }
            return false;
        }

        TArray &operator=(const std::initializer_list<T> &list) {
            clear();
            add(list);
            return *this;
        }

        TArray &operator=(const TArray &other) {
            if (this == &other) {
                return *this;
            }

            clear();
            add(other);
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

        void clear() {
            for (uint32 i = 0; i < mSize; i++) {
                mBuffer[i].~T();
            }
            mSize = 0;
        }

        /** Clear container and deallocate memory */
        void releaseInternalMemory() {
            if (mBuffer) {
                clear();
                mAlloc.free(mBuffer);
                mBuffer = nullptr;
                mCapacity = 0;
                mSize = 0;
            }
        }

        template<typename E = TEquals<T>>
        bool getIndexOf(const T &element, uint32 &index) const {
            E eq;
            for (uint32 i = 0; i < getSize(); i++) {
                if (eq(mBuffer[i], element)) {
                    index = i;
                    return true;
                }
            }

            return false;
        }

        bool isEmpty() const { return mSize == 0; }
        bool isNotEmpty() const { return mSize > 0; }

        T *getData() { return mBuffer; }
        const T *getData() const { return mBuffer; }

        uint32 getSize() const { return mSize; }
        uint32 getCapacity() const { return mCapacity; }

        template<typename H = THash<T>>
        uint32 hash() const {
            H hashGenerator;
            uint64 h = 0;
            for (const auto &a: *this) {
                h += hashGenerator(a);
            }

            return Crc32::hash(&h, sizeof(h));
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
                if (mIndex + 1 < mArray->getSize()) {
                    mIndex += 1;
                }
                else {
                    // If cannot increment - nullify the iterator
                    mArray = nullptr;
                    mIndex = 0;
                }
            }

            void remove() {
                if (mArray != nullptr && mIndex < mArray->getSize()) {
                    mArray->remove(mIndex);

                    // If after index no more elements - nullify the iterator
                    if (mIndex >= mArray->getSize()) {
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

        void expand() {
            if (mCapacity == 0) {
                mCapacity = INITIAL_CAPACITY;
                mBuffer = (T *) mAlloc.allocate(mCapacity * sizeof(T));
            } else {
                auto newCapacity = mCapacity * FACTOR;
                auto newBuffer = (T *) mAlloc.allocate(newCapacity * sizeof(T));
                Memory::copy(newBuffer, mBuffer, mSize * sizeof(T));
                mAlloc.free(mBuffer);
                mBuffer = newBuffer;
                mCapacity = newCapacity;
            }
        }

        void expand(uint32 capacity) {
            if (mCapacity == 0) {
                mCapacity = INITIAL_CAPACITY;
                // Expand accordingly to size factor to avoid unintended small allocations
                while (mCapacity < capacity) {
                    mCapacity *= FACTOR;
                }

                mBuffer = (T *) mAlloc.allocate(mCapacity * sizeof(T));
            } else {
                auto newCapacity = mCapacity;
                // Expand accordingly to size factor to avoid unintended small allocations
                while (newCapacity < capacity) {
                    newCapacity *= FACTOR;
                }

                auto newBuffer = (T *) mAlloc.allocate(newCapacity * sizeof(T));
                Memory::copy(newBuffer, mBuffer, mSize * sizeof(T));
                mAlloc.free(mBuffer);
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