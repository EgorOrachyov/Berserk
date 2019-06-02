//
// Created by Egor Orachyov on 02.06.2019.
//

#ifndef BERSERK_TARRAY_H
#define BERSERK_TARRAY_H

#include <Object/NewDelete.h>
#include <Containers/TList.h>
#include <Memory/IAllocator.h>

namespace Berserk
{

    /**
     * Dynamically expandable array list for elements of type T.
     * Automatically expands in the add method whether it does not have enough
     * space in the internal buffer. Provides iteration mechanism for elements for using in for loop.
     *
     * @tparam T Template type for elements of the array
     */
    template <typename T>
    class TArray : public TList<T>
    {
    public:

        /** Default required initial size */
        static const uint32 DEFAULT_CAPACITY = 16;

        /** Minimal initial size */
        static const uint32 MINIMAL_CAPACITY = 2;

        /** Expand factor for internal buffer allocations */
        static const float32 DEFAULT_EXPAND_FACTOR;

    public:

        GENERATE_NEW_DELETE(TArray);

        /**
         * Creates new buffer and allocates default memory count
         * @param initialCapacity To preallocate in buffer
         * @param allocator Allocator for internal buffer
         */
        explicit TArray(IAllocator& allocator = Allocator::getSingleton())
                : mAllocator(allocator), mSize(0), mCapacity(DEFAULT_CAPACITY), mCurrent(0)
        {
            mBuffer = (T*) mAllocator.allocate(mCapacity * sizeof(T));
        }

        /**
         * Creates new buffer and allocates specified memory count
         * @param initialCapacity To preallocate in buffer
         * @param allocator Allocator for internal buffer
         */
        explicit TArray(uint32 initialCapacity, IAllocator& allocator = Allocator::getSingleton())
                : mAllocator(allocator), mSize(0), mCapacity(initialCapacity), mCurrent(0)
        {
            assert(initialCapacity >= MINIMAL_CAPACITY);
            mBuffer = (T*) mAllocator.allocate(mCapacity * sizeof(T));
        }

        /** Prohibited */
        TArray(const TArray& array) = delete;

        /** Prohibited */
        TArray(const TArray&& array) = delete;

        ~TArray()
        {
            if (mBuffer != nullptr)
            {
                clear();
                mAllocator.free(mBuffer);
                mSize = 0;
                mCapacity = 0;
                mCurrent = 0;
            }
        }

        /** @copydoc TList::add() */
        void add(const T &element) override {
            if (mSize == mCapacity) expand();
            memcpy(&mBuffer[mSize++], &element, sizeof(T));
        }

        /** @copydoc TList::add(container) */
        void add(const TList<T> &container) override {
            uint32 newSize = mSize + container.getSize();
            if (newSize > mCapacity) expand(newSize);

            for (T* i = container.begin(); i != nullptr; i = container.next()) {
                add(*i);
            }
        }

        /** @copydoc TList::get() */
        T &get(uint32 index) const override {
            assert(index < mSize);
            return mBuffer[index];
        }

        /** @copydoc TList::remove() */
        void remove(uint32 index) override {
            assert(index < mSize);
            mBuffer[index].~T();
            mSize -= 1;
            if (mSize == index) return;
            else memcpy(&mBuffer[index], &mBuffer[mSize], sizeof(T));
        }

        /** @copydoc TList::clear() */
        void clear() override {
            for (uint32 i = 0; i < mSize; i++)
            {
                mBuffer[i].~T();
            }
            mSize = 0;
        }

        /** @copydoc TList::sort() */
        void sort(bool (*predicate)(const T &, const T &)) override {
            sort(0, mSize - 1, predicate);
        }

        /** @copydoc TList::getSize() */
        uint32 getSize() const override {
            return mSize;
        }

        /** @copydoc TList::getCapacity() */
        uint32 getCapacity() const override {
            return mCapacity;
        }

        /** @copydoc TList::getMemoryUsage() */
        uint32 getMemoryUsage() const override {
            return mCapacity * sizeof(T);
        }

        /** @copydoc TIterator::begin() */
        T *begin() const override
        {
            if (mSize > 0)
            {
                mCurrent = 0;
                return &mBuffer[0];
            }
            else
            {
                return nullptr;
            }
        }

        /** @copydoc TIterator::next() */
        T *next() const override
        {
            if (mCurrent + 1 < mSize)
            {
                mCurrent += 1;
                return &mBuffer[mCurrent];
            }
            else
            {
                return nullptr;
            }
        }

    private:

        /** Get new storage of bigger size */
        void expand()
        {
            T* oldBuffer = mBuffer;
            mCapacity = (uint32) (DEFAULT_EXPAND_FACTOR * (float32)mCapacity);
            mBuffer = (T*) mAllocator.allocate(mCapacity * sizeof(T));
            memcpy(mBuffer, oldBuffer, mSize * sizeof(T));
            mAllocator.free(oldBuffer);
        }

        /** Get new storage of bigger size with desired capacity */
        void expand(uint32 desired)
        {
            if (desired <= mCapacity) return;

            T* oldBuffer = mBuffer;
            mCapacity = desired;
            mBuffer = (T*) mAllocator.allocate(mCapacity * sizeof(T));
            memcpy(mBuffer, oldBuffer, mSize * sizeof(T));
            mAllocator.free(oldBuffer);
        }

        /** [Quick-sort internal] in 'operator <' order for objects */
        void sort(int32 left, int32 right, bool (*predicate)(const T &, const T &))
        {
            const int32 STOP_RECURSIVE_SORT = 8;

            if (right > left)
            {
                if (right - left <= STOP_RECURSIVE_SORT)
                {
                    uint32 end = 0;

                    for (int32 i = left; i < right; i++)
                    {
                        end += 1;

                        for (int32 j = left; j <= right - end; j++)
                        {
                            if (predicate(mBuffer[j+1], mBuffer[j]))
                            {
                                swap(j + 1, j);
                            }
                        }
                    }

                    return;
                }
                else
                {
                    auto c = right;
                    auto j = left;
                    for (int32 i = left; i <= right - 1; i++)
                    {
                        if (predicate(mBuffer[i], mBuffer[c]))
                        {
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
        void swap(int32 i, int32 j)
        {
            char buffer[sizeof(T)];

            memcpy(buffer, &mBuffer[i], sizeof(T));
            memcpy(&mBuffer[i], &mBuffer[j], sizeof(T));
            memcpy(&mBuffer[j], buffer, sizeof(T));
        }

    private:

        /** Allocator for internal buffer */
        IAllocator& mAllocator;

        /** Buffer with elements */
        T* mBuffer;

        /** Current size */
        uint32 mSize;

        /** Capacity without expand call */
        uint32 mCapacity;

        /** For iterating */
        mutable uint32 mCurrent;

    };

    template <typename T>
    const float32 TArray<T>::DEFAULT_EXPAND_FACTOR = 1.5f;

} // namespace Berserk

#endif //BERSERK_TARRAY_H
