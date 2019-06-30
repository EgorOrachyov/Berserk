//
// Created by Egor Orachyov on 02.06.2019.
//

#ifndef BERSERK_TARRAY_H
#define BERSERK_TARRAY_H

#include <Misc/AssertDev.h>
#include <Misc/NewDelete.h>
#include <Containers/TList.h>
#include <Memory/Allocator.h>
#include <Math/MathUtility.h>
#include <Serialization/ArchiveWriter.h>
#include <Serialization/ArchiveReader.h>

namespace Berserk
{

    /**
     * Dynamically expandable array list for elements of type T.
     *
     * Elements should be of re-allocatable type and support safe memory move without copy-constructor.
     * Automatically expands in the add method whether it does not have enough space in the internal buffer. 
     * Provides iteration mechanism for elements for using in for loop.
     *
     * @tparam T Template type for elements of the array
     */
    template <typename T>
    class CORE_EXPORT TArray : public TList<T>
    {
    public:

        /** Default required initial size */
        static const uint32 DEFAULT_CAPACITY = 16;

        /** Minimal initial size */
        static const uint32 MINIMAL_CAPACITY = 2;

        /** Expand factor for internal buffer allocations */
        static const float32 DEFAULT_EXPAND_FACTOR;

        /** When to stop recursive array split for qsort */
        static const uint32 STOP_RECURSIVE_SORT = 8;

        /** Compare predicate type */
        typedef bool (*Predicate)(const T& a, const T& b);

    public:

        GENERATE_NEW_DELETE(TArray);

        /**
         * Initialize array and do not pre-allocate memory for buffer
         * @param initialCapacity To preallocate in buffer
         * @param allocator Allocator for internal buffer
         */
        explicit TArray(IAllocator& allocator = Allocator::get())
                : mAllocator(allocator), mSize(0), mCapacity(0), mCurrent(0)
        {
            mBuffer = nullptr;
        }

        /**
         * Creates new buffer and allocates specified memory count
         * @param initialCapacity To preallocate in buffer
         * @param allocator Allocator for internal buffer
         */
        explicit TArray(uint32 initialCapacity, IAllocator& allocator = Allocator::get())
                : mAllocator(allocator), mSize(0), mCapacity(initialCapacity), mCurrent(0)
        {
            assertion_dev(initialCapacity >= MINIMAL_CAPACITY);
            mBuffer = (T*) mAllocator.allocate(mCapacity * sizeof(T));
        }

        /**
         * Initialize container data from raw C-style array
         * @param array Pointer to buffer with elements
         * @param count Num of elements in buffer
         * @param allocator Allocator for internal buffer
         */
        TArray(const T* array, uint32 count, IAllocator& allocator = Allocator::get())
                : mAllocator(allocator), mCapacity(Math::max(count, DEFAULT_CAPACITY)), mSize(count), mCurrent(0)
        {
            assertion_dev(array);
            mBuffer = (T*) mAllocator.allocate(mCapacity * sizeof(T));
            memcpy(mBuffer, array, mSize * sizeof(T));
        }

        /** Prohibited */
        TArray(TArray& array)
                : mAllocator(array.mAllocator),
                  mSize(array.mSize),
                  mCapacity(array.mCapacity),
                  mCurrent(array.mCurrent),
                  mBuffer(array.mBuffer)
        {
            array.mSize = 0;
            array.mCapacity = 0;
            array.mCurrent = 0;
            array.mBuffer = nullptr;
        }

        /** Prohibited */
        TArray(TArray&& array)
                : mAllocator(array.mAllocator),
                  mSize(array.mSize),
                  mCapacity(array.mCapacity),
                  mCurrent(array.mCurrent),
                  mBuffer(array.mBuffer)
        {
            array.mSize = 0;
            array.mCapacity = 0;
            array.mCurrent = 0;
            array.mBuffer = nullptr;
        }

        ~TArray() override
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

        /**
         * Allows to ensure capacity to be able to add
         * desired count of the elements
         */
        void ensureCapacity(uint32 desired)
        {
            if (mSize + desired > mCapacity)
            {
                expand(mSize + desired);
            }
        }

        /** @copydoc TList::add() */
        void add(const T &element) override
        {
            if (mSize == mCapacity) expand();
            T* raw_element = new (&mBuffer[mSize++]) T((T&)element);
        }

        /** @copydoc TList::addUninitialized() */
        T *addUninitialized() override 
        {
            if (mSize == mCapacity) expand();
            return &mBuffer[mSize++];
        }

        /**
         * Adds specified number of elements in the end of the array without
         * initialization (i.e. without calling default constructor)
         * @param count Number of elements to add
         * @return Size of the container before the addition or
         *         index of the first added uninitialized element
         */
        uint32 addUninitialized(uint32 count)
        {
            uint32 newSize = mSize + count;
            if (newSize > mCapacity) expand(newSize);

            uint32 oldSize = mSize;
            mSize = newSize;
            return oldSize;
        }

        /** @copydoc TList::add(container) */
        void append(const TList<T> &container) override
        {
            uint32 newSize = mSize + container.getSize();
            if (newSize > mCapacity) expand(newSize);

            for (T* i = container.begin(); i != nullptr; i = container.next())
            {
                add(*i);
            }
        }

        /** @copydoc TList::add(array, count) */
        void append(const T *array, uint32 count) override
        {
            uint32 newSize = mSize + count;
            if (newSize > mCapacity) expand(newSize);

            for (uint32 i = 0; i < count; i++)
            {
                add(array[i]);
            }
        }

        /** @copydoc TList::get() */
        T &get(uint32 index) const override
        {
            rangeCheck(index);
            return mBuffer[index];
        }

        /** @copydoc TList::remove() */
        void remove(uint32 index) override
        {
            rangeCheck(index);
            mBuffer[index].~T();
            mSize -= 1;
            if (mSize == index) return;
            else memcpy(&mBuffer[index], &mBuffer[mSize], sizeof(T));
        }

        /** @copydoc TList::clear() */
        void clear() override
        {
            for (uint32 i = 0; i < mSize; i++)
            {
                mBuffer[i].~T();
            }
            mSize = 0;
        }

        /** @copydoc TList::clearNoDestructorCall() */
        void clearNoDestructorCall() override
        {
            mSize = 0;
        }

        /** @copydoc TList::sort() */
        void sort(Predicate predicate) override
        {
            sort(0, mSize - 1, predicate);
        }

        /** @copydoc TList::getSize() */
        uint32 getSize() const override
        {
            return mSize;
        }

        /**
         * Capacity for pre-allocatable containers
         * Or capacity of internal memory allocator
         *
         * @return Max number of the elements, which could be stored
         *         without memory allocation expanding
         */
        uint32 getCapacity() const
        {
            return mCapacity;
        }

        /** @copydoc TList::getMemoryUsage() */
        uint32 getMemoryUsage() const override
        {
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

        /** @return Allocator for this container */
        IAllocator& getAllocator() const
        {
            return mAllocator;
        }

        /**
         * Serialization operator.
         *
         * Allows to store the container content to the archive and then
         * load this content from the archive to an arbitrary array of the same type T.
         *
         * @param archive Archive to serialize this container data
         * @param array Array to serialize
         * @return Passing next the giver as param archive
         */
        friend ArchiveWriter& operator<<(ArchiveWriter& archive, const TArray& array)
        {
            archive << array.mSize;

            /** Use per object serialization (in case where it could be complex objects) */
            for (uint32 i = 0; i < array.mSize; i++)
            {
                archive << array.mBuffer[i];
            }

            return archive;
        }

        /**
         * Deserialization operator.
         *
         * Allows to load previously saved container content from
         * archive to an arbitrary array of the same type T.
         *
         * @param archive Archive to deserialize this container data
         * @param array Array to store deserialized data
         * @return Passing next the giver as param archive
         */
        friend ArchiveReader& operator>>(ArchiveReader& archive, TArray& array)
        {
            uint32 savedElementsNum;
            archive >> savedElementsNum;

            for (uint32 i = 0; i < savedElementsNum; i++)
            {
                T element;
                archive >> element;
                array.add(element);
            }

            return archive;
        }

    private:

        /** Assert fail on index out of range */
        void rangeCheck(uint32 index) const
        {
            assertion_dev(index < mSize);
        }

        /** Get new storage of bigger size */
        void expand()
        {
            if (mBuffer == nullptr)
            {
                mCapacity = DEFAULT_CAPACITY;
                mBuffer = (T*) mAllocator.allocate(mCapacity * sizeof(T));
            }
            else
            {
                T* oldBuffer = mBuffer;
                mCapacity = (uint32) (DEFAULT_EXPAND_FACTOR * (float32)mCapacity);
                mBuffer = (T*) mAllocator.allocate(mCapacity * sizeof(T));
                memcpy(mBuffer, oldBuffer, mSize * sizeof(T));
                mAllocator.free(oldBuffer);
            }
        }

        /** Get new storage of bigger size with desired capacity */
        void expand(uint32 desired)
        {
            if (desired <= mCapacity) return;

            if (mBuffer == nullptr)
            {
                mCapacity = desired;
                mBuffer = (T*) mAllocator.allocate(mCapacity * sizeof(T));
            }
            else
            {
                T* oldBuffer = mBuffer;
                mCapacity = desired;
                mBuffer = (T*) mAllocator.allocate(mCapacity * sizeof(T));
                memcpy(mBuffer, oldBuffer, mSize * sizeof(T));
                mAllocator.free(oldBuffer);
            }
        }

        /** [Quick-sort internal] in 'operator <' order for objects */
        void sort(int32 left, int32 right, Predicate predicate)
        {
            if (right > left)
            {
                if (right - left <= STOP_RECURSIVE_SORT)
                {
                    // Bubble sort here for small amount of data
                    auto end = 0;
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
                    // Quick sort with the right element as pivot
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

    public:

        class Iterator : public TIterator<T>
        {
        public:

            Iterator()
            {

            }

            Iterator(T* buffer, uint32 size) : mBuffer(buffer), mSize(size)
            {

            }

            ~Iterator() override = default;

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

            /** Data elements */
            T* mBuffer = nullptr;

            /** Num of elements in the array */
            uint32 mSize = 0;

            /** Index of the current element */
            mutable uint32 mCurrent = 0;

        };

        /**
         * Creates special TArray iterator
         * @return Instance (to be copied)
         */
        Iterator createIterator() const
        {
            return Iterator(mBuffer, mSize);
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
