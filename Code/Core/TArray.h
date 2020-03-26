/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TARRAY_H
#define BERSERK_TARRAY_H

#include <ErrorMacro.h>
#include <TIterable.h>
#include <TPredicates.h>
#include <Alloc.h>
#include <IO/Archive.h>
#include <Platform/Memory.h>

namespace Berserk {

    template <typename T>
    class TArray final : public TIterable<T> {
    public:

        static const uint32 INITIAL_CAPACITY = 2;
        static const uint32 FACTOR = 2;

        explicit TArray(IAlloc& alloc = IAlloc::getSingleton()) noexcept : mAlloc(&alloc) {}
        TArray(const std::initializer_list<T> &list) : TArray<T>() {
            add(list);
        }
        TArray(const TArray& other) : TArray<T>() {
            mAlloc = other.mAlloc;
            add(other);
        }
        TArray(TArray<T> &&other) noexcept
            : mAlloc(other.mAlloc),
              mBuffer(other.mBuffer),
              mCapacity(other.mCapacity),
              mSize(other.mSize) {
            other.mAlloc = nullptr;
            other.mBuffer = nullptr;
            other.mSize = 0;
            other.mCapacity = 0;
        }
        ~TArray() override {
            if (mBuffer) {
                clear();
                mAlloc->free(mBuffer);
                mBuffer = nullptr;
                mCapacity = 0;
                mSize = 0;
                mAlloc = 0;
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

        template <typename ... TArgs>
        T& emplace(TArgs && ... args) {
            if (mSize >= mCapacity) {
                expand();
            }

            T* obj = new(&mBuffer[mSize]) T(std::forward<TArgs>(args)...);
            mSize += 1;

            return *obj;
        }

        T& add(const T& element) {
            if (mSize >= mCapacity) {
                expand();
            }

            T* obj = new(&mBuffer[mSize]) T(element);
            mSize += 1;

            return *obj;
        }
        void add(const std::initializer_list<T> &list) {
            ensureToAdd(list.size());

            for (const auto& e: list) {
                new(&mBuffer[mSize]) T(e);
                mSize += 1;
            }
        }
        void add(const TArray &other) {
            ensureToAdd(other.mSize);

            for (uint32 i = 0; i < other.mSize; i++) {
                new(&mBuffer[mSize]) T(other[i]);
                mSize += 1;
            }
        }
        void add(const T* buffer, uint32 count) {
            ensureToAdd(count);

            for (uint32 i = 0; i < count; i++) {
                new(&mBuffer[mSize]) T(buffer[i]);
                mSize += 1;
            }
        }
        void remove(uint32 index) {
            if (index >= mSize) {
                BERSERK_ERROR_RET("Index out of range")
            }

            mBuffer[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Memory::copy(&mBuffer[index], &mBuffer[index + 1], sizeof(T) * (mSize - index));
            }
        }
        template <typename E = TEquals<T>>
        void removeElement(const T& toRemove) {
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
        template <typename E = TEquals<T>>
        void removeElementUnordered(const T& toRemove) {
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
        void removeElementPtr(const T* toRemove) {
            uint32 i = 0;
            while (i < mSize) {
                if (&mBuffer[i] == toRemove) {
                    remove(i);
                    break;
                }

                i += 1;
            }
        }
        void removeUnordered(uint32 index) {
            if (index >= mSize) {
                BERSERK_ERROR_RET("Index out of range")
            }

            mBuffer[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Memory::copy(&mBuffer[index], &mBuffer[mSize], sizeof(T));
            }
        }

        template <typename E = TEquals<T>>
        bool contains(const T& element) {
            E eq;
            for (const auto& e: *this) {
                if (eq(e,element))
                    return true;
            }
            return false;
        }

        TArray& operator*=(uint32 N) {
            ensureCapacity(N * mSize);

            if (N == 0) {
                clear();
            }
            else {
                auto oldSize = mSize;
                for (uint32 i = 1; i < N; i++) {
                    for (uint32 j = 0; j < oldSize; j++) {
                        new(&mBuffer[mSize]) T(mBuffer[j]);
                        mSize += 1;
                    }
                }
            }

            return *this;
        }
        TArray& operator+=(const T& element) {
            add(element);
            return *this;
        }
        TArray& operator+=(const std::initializer_list<T> &list) {
            add(list);
            return *this;
        }
        TArray& operator+=(const TArray& other) {
            add(other);
            return *this;
        }
        TArray& operator=(const std::initializer_list<T> &list) {
            clear();
            add(list);
            return *this;
        }
        TArray& operator=(const TArray& other) {
            clear();
            add(other);
            return *this;
        }
        TArray& operator=(TArray&& other) noexcept {
            clear();
            mAlloc = other.mAlloc;
            mBuffer = other.mBuffer;
            mCapacity = other.mCapacity;
            mSize = other.mSize;
            other.mAlloc = nullptr;
            other.mBuffer = nullptr;
            other.mSize = 0;
            other.mCapacity = 0;
            return *this;
        }
        TArray operator+(const TArray& other) const {
            TArray<T> result(*mAlloc);
            result.ensureToAdd(mSize + other.mSize);
            result += *this;
            result += other;
            return result;
        }
        T& operator[](uint32 index) {
            if (index >= mSize) {
                BERSERK_ERROR_FAIL("Index out of range")
            }

            return mBuffer[index];
        }
        const T& operator[](uint32 index) const {
            if (index >= mSize) {
                BERSERK_ERROR_FAIL("Index out of range")
            }

            return mBuffer[index];
        }

        void resizeExplicitly(uint32 size) {
            if (size <= mCapacity) {
                mSize = size;
            }
        }
        void clear() {
            for (uint32 i = 0; i < mSize; i++) {
                mBuffer[i].~T();
            }
            mSize = 0;
        }
        void clearNoDestructorCall() {
            mSize = 0;
        }

        void forEach(const Function<void(T&)> &function) override {
            for (uint32 i = 0; i < mSize; i++) {
                function(mBuffer[i]);
            }
        }
        void forEach(const Function<void(const T&)> &function) const override {
            for (uint32 i = 0; i < mSize; i++) {
                function(mBuffer[i]);
            }
        }

        T* data() { return mBuffer; }
        const T* data() const { return mBuffer; }
        uint32 size() const { return mSize; }
        uint32 capacity() const { return mCapacity; }

        const T* begin() const {
            return mBuffer;
        }
        const T* end() const {
            return mBuffer + mSize;
        }

        T* begin() {
            return mBuffer;
        }
        T* end() {
            return mBuffer + mSize;
        }

    private:

        void expand() {
            if (mCapacity == 0) {
                mCapacity = INITIAL_CAPACITY;
                mBuffer = (T*) mAlloc->allocate(mCapacity * sizeof(T));
            }
            else {
                auto newCapacity = mCapacity * FACTOR;
                auto newBuffer = (T*) mAlloc->allocate(newCapacity * sizeof(T));
                Memory::copy(newBuffer, mBuffer, mSize * sizeof(T));
                mAlloc->free(mBuffer);
                mBuffer = newBuffer;
                mCapacity = newCapacity;
            }
        }

        void expand(uint32 capacity) {
            if (mCapacity == 0) {
                mCapacity = capacity;
                mBuffer = (T*) mAlloc->allocate(mCapacity * sizeof(T));
            }
            else {
                auto newCapacity = capacity;
                auto newBuffer = (T*) mAlloc->allocate(newCapacity * sizeof(T));
                Memory::copy(newBuffer, mBuffer, mSize * sizeof(T));
                mAlloc->free(mBuffer);
                mBuffer = newBuffer;
                mCapacity = newCapacity;
            }
        }

        /**
         * Serialize an array of objects in an archive.
         * @note T objects must support serialization operation '<<'
         * @param archive Archive to serialize data
         * @param array Array of the elements of type T to be serialized
         * @return archive
         */
        friend Archive& operator<<(Archive& archive, const TArray<T> &array) {
            auto elements = array.size();
            archive << elements;

            for (const auto& e: array) {
                archive << e;
            }

            return archive;
        }

        /**
         * Deserialize an array of objects in from an archive.
         * @note T objects must support serialization operation '>>'
         * @param archive Archive to deserialize data
         * @param array Array of the elements of type T to be deserialized
         * @return archive
         */
        friend Archive& operator>>(Archive& archive, TArray<T> &array) {
            uint32 elements = 0;
            archive >> elements;
            array.ensureCapacity(elements);

            for (uint32 i = 0; i < elements; i++) {
                T object;
                archive >> object;
                array.emplace(std::move(object));
            }

            return archive;
        }

    private:
        IAlloc* mAlloc = nullptr;
        T* mBuffer = nullptr;
        uint32 mCapacity = 0;
        uint32 mSize = 0;
    };

}

#endif //BERSERK_TARRAY_H