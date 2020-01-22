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
#include <Alloc.h>
#include <Platform/Memory.h>

namespace Berserk {

    template <typename T>
    class TArray final : public TIterable<T> {
    public:

        static const uint32 INITIAL_CAPACITY = 4;
        static const uint32 FACTOR = 2;

        explicit TArray(Alloc& alloc = Alloc::getSingleton()) : mAlloc(&alloc) {}
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

    private:
        Alloc* mAlloc = nullptr;
        T* mBuffer = nullptr;
        uint32 mCapacity = 0;
        uint32 mSize = 0;
    };

}

#endif //BERSERK_TARRAY_H