/**********************************************************************************/
/* This file is part of:                                                          */
/* Berserk Engine project                                                         */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018-2019 Egor Orachyov                                          */
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

#ifndef BERSERK_TARRAY_H
#define BERSERK_TARRAY_H

#include <Containers/TList.h>
#include <Memory/Allocator.h>
#include <Misc/MathUtils.h>

namespace Berserk
{

    /**
     * Dynamically expandable array list for elements of type T.
     *
     * Elements should be of re-allocatable type and support safe memory movement.
     * Automatically expands in the add method whether it does not have enough space in the internal buffer. 
     * Provides iteration mechanism for elements for using in for loop.
     *
     * @tparam T Template type for elements of the array
     */
    template <typename T>
    class TArray : public TList<T>
    {
    public:

        /** Default required initial size */
        static const uint32 DEFAULT_CAPACITY = 8;

        /** Minimal initial size */
        static const uint32 MINIMAL_CAPACITY = 2;

        /** Expand factor for internal buffer allocations */
        static const float32 DEFAULT_EXPAND_FACTOR;

        /** When to stop recursive array split for qsort */
        static const uint32 STOP_RECURSIVE_SORT = 8;

    public:

        /**
         * Initialize array and do not pre-allocate memory for buffer
         * @param initialCapacity To preallocate internal buffer
         * @param allocator Allocator for internal buffer
         */
        explicit TArray(IAllocator& allocator = Allocator::getSingleton())
                : mAllocator(allocator), mSize(0), mCapacity(0), mCurrent(0) {
            mBuffer = nullptr;
        }

        /**
         * Creates new buffer and allocates specified memory count
         * @param initialCapacity To preallocate internal buffer
         * @param allocator Allocator for internal buffer
         */
        explicit TArray(uint32 initialCapacity, IAllocator& allocator = Allocator::getSingleton())
                : mAllocator(allocator), mSize(0), mCapacity(initialCapacity), mCurrent(0) {
            DEV_ERROR_CONDITION(initialCapacity >= MINIMAL_CAPACITY, "Does not satisfy min size limitation");
            mBuffer = (T*) mAllocator.malloc(mCapacity * sizeof(T));
        }

        /**
         * Initialize container data from raw C-style array
         * @param array Pointer to buffer with elements
         * @param count Num of elements in buffer
         * @param allocator Allocator for internal buffer
         */
        TArray(const T* array, uint32 count, IAllocator& allocator = Allocator::getSingleton())
                : mAllocator(allocator), mCapacity(MathUtils::max(count, DEFAULT_CAPACITY)), mSize(count), mCurrent(0) {
            DEV_ERROR_CONDITION(array, "");
            mBuffer = (T*) mAllocator.malloc(mCapacity * sizeof(T));
            Memory::copy(mBuffer, array, mSize * sizeof(T));
        }

        /** Init array from list */
        TArray(const std::initializer_list<T>& list) : TArray<T>(Allocator::getSingleton()) {
            append(list);
        }

        /** Copy content of source array */
        TArray(const TArray& array) : TArray(array.mAllocator) {
            if (array.mSize > 0) {
                append(array.mBuffer, array.mSize);
            }
        }

        /** Move array content to this array */
        TArray(TArray&& array) noexcept
                : mAllocator(array.mAllocator),
                  mSize(array.mSize),
                  mCapacity(array.mCapacity),
                  mCurrent(array.mCurrent),
                  mBuffer(array.mBuffer) {
            array.mSize = 0;
            array.mCapacity = 0;
            array.mCurrent = 0;
            array.mBuffer = nullptr;
        }

        ~TArray() override {
            if (mBuffer != nullptr) {
                clear();
                mAllocator.free(mBuffer);
                mSize = 0;
                mCapacity = 0;
                mCurrent = 0;
                mBuffer = nullptr;

            }
        }

        /**
         * Allows to ensure capacity to be able to add
         * desired count of the elements
         */
        void ensureCapacity(uint32 desired) {
            if (mSize + desired > mCapacity) {
                expand(mSize + desired);
            }
        }

        /** @copydoc TList::add() */
        void add(const T &element) override {
            if (mSize == mCapacity) expand();
            T* raw_element = new (&mBuffer[mSize++]) T((T&)element);
        }

        /** @copydoc TList::addUninitialized() */
        T *addUninitialized() override {
            if (mSize == mCapacity) expand();
            return &mBuffer[mSize++];
        }

        /**
         * Adds specified number of elements in the end of the array without
         * initialization (i.e. without calling default constructor)
         * @param count Number of elements to add
         * @return Size of the container before the addition
         *         (== index of the first added uninitialized element)
         */
        uint32 addUninitialized(uint32 count) {
            uint32 newSize = mSize + count;
            if (newSize > mCapacity) expand(newSize);

            uint32 oldSize = mSize;
            mSize = newSize;
            return oldSize;
        }

        /** @copydoc TList::add(array, count) */
        void append(const T *array, uint32 count) override {
            uint32 newSize = mSize + count;
            if (newSize > mCapacity) expand(newSize);

            for (uint32 i = 0; i < count; i++) {
                add(array[i]);
            }
        }

        /** @copydoc TList::add(list) */
        void append(const std::initializer_list<T>& list) override {
            ensureCapacity(list.size());
            for (const T& element : list) {
                add(element);
            }
        }

        /** @copydoc TList::get() */
        T &get(uint32 index) const override {
            rangeCheck(index);
            return mBuffer[index];
        }

        /** @copydoc TList::find() */
        TVariant<T*> find(const typename TPredicate::Satisfy<T>::type &predicate) const override {
            for (uint32 i = 0; i < mSize; i++) {
                if (predicate(mBuffer[i])) {
                    T* ptr = &mBuffer[i];
                    return TVariant<T*>(ptr);
                }
            }

            return TVariant<T*>();
        }

        /** @copydoc TList::remove() */
        void remove(uint32 index) override {
            rangeCheck(index);
            mBuffer[index].~T();
            mSize -= 1;
            if (mSize == index) return;
            else Memory::copy(&mBuffer[index], &mBuffer[mSize], sizeof(T));
        }

        /** Remove element with index an save the order */
        void removeOrdered(uint32 index) {
            rangeCheck(index);
            mBuffer[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Memory::copy(&mBuffer[index], &mBuffer[index + 1], sizeof(T) * (mSize - index));
            }
        }

        /** @copydoc TList::clear() */
        void clear() override {
            for (uint32 i = 0; i < mSize; i++) {
                mBuffer[i].~T();
            }
            mSize = 0;
        }

        /** @copydoc TList::clearNoDestructorCall() */
        void clearNoDestructorCall() override {
            mSize = 0;
        }

        /** @copydoc TList::sort() */
        void sort(const typename TPredicate::Compare<T>::type &predicate) override {
            sort(0, mSize - 1, predicate);
        }

        /** @return Pointer to raw internal buffer */
        T* getRawBuffer() const {
            return mBuffer;
        }

        /** @copydoc TList::getSize() */
        uint32 getSize() const override {
            return mSize;
        }

        /**
         * Capacity for pre-allocatable containers
         * Or capacity of internal memory allocator
         *
         * @return Max number of the elements, which could be stored
         *         without memory allocation expanding
         */
        uint32 getCapacity() const {
            return mCapacity;
        }

        /** @copydoc TList::getMemoryUsage() */
        uint32 getMemoryUsage() const override {
            return mCapacity * sizeof(T);
        }

        /** @return Allocator for this container */
        IAllocator& getAllocator() const {
            return mAllocator;
        }

        /** Foreach loop */
        T* begin() const {
            return (mSize > 0 ? mBuffer : nullptr);
        }

        /** Foreach loop */
        T* end() const {
            return (mSize > 0 ? mBuffer + mSize : nullptr);
        }

        /** @copydoc TIterable::foreach() */
        void forEach(const typename TPredicate::Consume<T>::type &function) override {
            for (const T& e: *this) {
                function(e);
            }
        }

    private:

        /** Assert fail on index out of range */
        void rangeCheck(uint32 index) const {
            DEV_ERROR_CONDITION(index < mSize, "Index out of bounds");
        }

        /** Get new storage of bigger size */
        void expand() {
            if (mBuffer == nullptr) {
                mCapacity = DEFAULT_CAPACITY;
                mBuffer = (T*) mAllocator.malloc(mCapacity * sizeof(T));
            }
            else {
                void* oldBuffer = mBuffer;
                mCapacity = (uint32) (DEFAULT_EXPAND_FACTOR * (float32)mCapacity);
                mBuffer = (T*) mAllocator.malloc(mCapacity * sizeof(T));
                Memory::copy(mBuffer, oldBuffer, mSize * sizeof(T));
                mAllocator.free(oldBuffer);
            }
        }

        /** Get new storage of bigger size with desired capacity */
        void expand(uint32 desiredSize) {
            if (desiredSize <= mCapacity) return;

            if (mBuffer == nullptr) {
                mCapacity = desiredSize;
                mBuffer = (T*) mAllocator.malloc(mCapacity * sizeof(T));
            }
            else {
                void* oldBuffer = mBuffer;
                mCapacity = desiredSize;
                mBuffer = (T*) mAllocator.malloc(mCapacity * sizeof(T));
                Memory::copy(mBuffer, oldBuffer, mSize * sizeof(T));
                mAllocator.free(oldBuffer);
            }
        }

        /** [Quick-sort internal] in 'operator <' order for objects */
        void sort(int32 left, int32 right, const typename TPredicate::Compare<T>::type &predicate) {
            if (right > left) {
                if (right - left <= STOP_RECURSIVE_SORT) {
                    // Bubble sort here for small amount of data
                    auto end = 0;
                    for (int32 i = left; i < right; i++) {
                        end += 1;
                        for (int32 j = left; j <= right - end; j++) {
                            if (predicate(mBuffer[j+1], mBuffer[j])) {
                                swap(j + 1, j);
                            }
                        }
                    }
                    return;
                }
                else {
                    // Quick sort with the right element as pivot
                    auto c = right;
                    auto j = left;
                    for (int32 i = left; i <= right - 1; i++) {
                        if (predicate(mBuffer[i], mBuffer[c])) {
                            swap(i, j);
                            j += 1;
                        }
                    }
                    swap(j, c);
                    sort(left, j - 1,  predicate);
                    sort(j + 1, right, predicate);
                }
            }
        }

        /** Swaps two elements in buffer via tmp storage */
        void swap(int32 i, int32 j) {
            char buffer[sizeof(T)];
            Memory::copy(buffer, &mBuffer[i], sizeof(T));
            Memory::copy(&mBuffer[i], &mBuffer[j], sizeof(T));
            Memory::copy(&mBuffer[j], buffer, sizeof(T));
        }

    private:

        /** Allocator for internal buffer */
        IAllocator& mAllocator;

        /** Buffer with elements */
        T* mBuffer = nullptr;

        /** Current size */
        uint32 mSize = 0;

        /** Capacity without expand call */
        uint32 mCapacity = 0;

        /** For iterating */
        mutable uint32 mCurrent = 0;

    };

    template <typename T>
    const float32 TArray<T>::DEFAULT_EXPAND_FACTOR = 1.5f;

} // namespace Berserk

#endif //BERSERK_TARRAY_H
