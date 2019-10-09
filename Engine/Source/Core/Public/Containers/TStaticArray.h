//
// Created by Egor Orachyov on 2019-10-07.
//

#ifndef BERSERK_TSTATICARRAY_H
#define BERSERK_TSTATICARRAY_H

#include <TComparator.h>
#include <Containers/TList.h>
#include <Misc/Error.h>
#include <HAL/Memory.h>

namespace Berserk {

    /**
     * Fixed capacity array. Allows to store its data on
     * the stack in the creator function frame.
     *
     * @tparam T Type of stored elements
     * @tparam CAPACITY Max num of stored elements
     * @tparam C Comparator class for equality (a == b)
     */
    template <typename T, uint32 CAPACITY = 16, typename C = TEquals<T>>
    class TStaticArray : public TList<T> {
    public:

        /** Stops recursive sort with loop */
        static const uint32 STOP_RECURSIVE_SORT = 8;

        static_assert(CAPACITY > 0, "Capacity must be more than 0");

        /** Creates empty array */
        TStaticArray() = default;

        /** Creates array from list initializer */
        TStaticArray(const std::initializer_list<T> &list) {
            auto elements = (T*) mBuffer;
            for (auto &data : list) {
                if (mSize >= CAPACITY) {
                    DEV_ERROR("Cannot add element, array is full");
                }

                new (&elements[mSize]) T(data);
                mSize += 1;
            }
        }

        /** Creates array from raw C-style buffer with elements */
        TStaticArray(const T* buffer, uint32 size) {
            DEV_ERROR_CONDITION(CAPACITY <= size, "Cannot create array from source of bigger size");

            auto elements = (T*) mBuffer;
            for (uint32 i = 0; i < size; i++) {
                new (&elements[mSize]) T(buffer[i]);
                mSize += 1;
            }
        }

        template <uint32 SOURCE_CAPACITY>
        TStaticArray(const TStaticArray<T,SOURCE_CAPACITY,C>& source) {
            DEV_ERROR_CONDITION(source.getSize() <= CAPACITY, "Cannot create array from source of bigger size");

            auto elements = (T*) mBuffer;
            for (const T& d: source) {
                new (&elements[mSize]) T(d);
                mSize += 1;
            }
        }

        template <uint32 SOURCE_CAPACITY>
        TStaticArray(TStaticArray<T,SOURCE_CAPACITY,C>&& source) {
            DEV_ERROR_CONDITION(source.getSize() <= CAPACITY, "Cannot create array from source of bigger size");

            mSize = source.getSize();
            Memory::copy(mBuffer, source.mBuffer, sizeof(T) * mSize);
            source.mSize = 0;
        }

        ~TStaticArray() {
            if (mSize > 0) {
                clear();
            }
        }

        /** @copydoc TList::add() */
        void add(const T &element) override {
            T* memory = addUninitialized();
            new (memory) T(element);

        }

        /** @copydoc TList::addUninitialized() */
        T *addUninitialized() override {
            if (mSize >= CAPACITY) {
                DEV_ERROR("Cannot add element, array is full");
            }

            auto index = mSize;
            mSize += 1;
            auto elements = (T*) mBuffer;
            return &elements[index];
        }

        /** @copydoc TList::append() */
        void append(const T *array, uint32 count) override {
            auto elements = (T*) mBuffer;
            for (int i = 0; i < count; i++) {
                if (mSize >= CAPACITY) {
                    DEV_ERROR("Cannot add element, array is full");
                }

                new (&elements[mSize]) T(array[count]);
                mSize += 1;
            }
        }

        /** @copydoc TList::append() */
        void append(const std::initializer_list<T> &list) override {
            auto elements = (T*) mBuffer;
            for (auto &data : list) {
                if (mSize >= CAPACITY) {
                    DEV_ERROR("Cannot add element, array is full");
                }

                new (&elements[mSize]) T(data);
                mSize += 1;
            }
        }

        /** @copydoc TList::get() */
        T &get(uint32 index) const override {
            if (index >= mSize) {
                DEV_ERROR("Index out of bounds [size: %u][index: %u]", mSize, index);
            }

            return getBuffer()[index];
        }

        /** @copydoc TList::find() */
        TVariant<T*> find(typename TPredicate::Satisfy<T>::type predicate) const {
            for (T& data : *this) {
                if (predicate(data)) {
                    T* ptr = &data;
                    return TVariant<T*>(ptr);
                }
            }

            return TVariant<T*>();
        }

        /** @copydoc TList::remove() */
        void remove(uint32 index) override {
            if (index >= mSize) {
                DEV_ERROR("Index out of bounds [size: %u][index: %u]", mSize, index);
            }

            auto elements = getBuffer();
            elements[index].~T();
            mSize -= 1;

            if (mSize == index) {
                return;
            } else {
                /** Copy last element on the place of the removed */
                Memory::copy(&elements[index], &elements[mSize], sizeof(T));
            }
        }

        /** @copydoc TList::clear() */
        void clear() override {
            auto elements = (T*) mBuffer;
            for (int32 i = 0; i < mSize; i++) {
                elements[i].~T();
            }
            mSize = 0;
        }

        /** @copydoc TList::clearNoDestructorCall() */
        void clearNoDestructorCall() override {
            mSize = 0;
        }

        /** @copydoc TList::sort() */
        void sort(typename TPredicate::Compare<T>::type predicate) override {
            if (mSize > 0) {
                sort(0, mSize - 1, predicate);
            }
        }

        /** @return Raw internal buffer */
        T* getBuffer() const {
            return (T*) mBuffer;
        }

        /** @copydoc TList::getSize() */
        uint32 getSize() const override {
            return mSize;
        }

        /** @copydoc TList::getMemoryUsage() */
        uint32 getMemoryUsage() const override {
            return sizeof(TStaticArray);
        }

        /** foreach loop */
        T* begin() const {
            return & getBuffer()[0];
        }

        /** foreach loop */
        T* end() const {
            return & getBuffer()[mSize];
        }

    private:

        /** [Quick-sort internal] in 'operator <' order for objects */
        void sort(int32 left, int32 right, typename TPredicate::Compare<T>::type predicate) {
            auto elements = getBuffer();

            if (right > left) {
                if (right - left <= STOP_RECURSIVE_SORT) {
                    // Bubble sort here for small amount of data
                    auto end = 0;
                    for (int32 i = left; i < right; i++) {
                        end += 1;
                        for (int32 j = left; j <= right - end; j++) {
                            if (predicate(elements[j+1], elements[j])) {
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
                        if (predicate(elements[i], elements[c])) {
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
            auto elements = getBuffer();

            memcpy(buffer, &elements[i], sizeof(T));
            memcpy(&elements[i], &elements[j], sizeof(T));
            memcpy(&elements[j], buffer, sizeof(T));
        }

    private:

        /** Current size */
        uint32 mSize = 0;

        /** Captured buffer */
        uint8 mBuffer[CAPACITY * sizeof(T)] = {};

    };

}

#endif //BERSERK_TSTATICARRAY_H