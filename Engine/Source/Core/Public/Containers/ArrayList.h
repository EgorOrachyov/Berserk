//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_ARRAYLIST_H
#define BERSERK_ARRAYLIST_H

#include "Public/Misc/Types.h"
#include "Public/Misc/Assert.h"
#include "Public/Memory/Allocator.h"
#include "Public/Logging/LogMacros.h"

namespace Berserk
{

    /**
     * Dynamically expandable array list for elements of type T.
     * Automatically expands in the add method whether it does not have enough
     * space in the internal buffer. Relies on default platform allocator.
     * Provides iteration mechanism for elements for using in for loop.
     *
     * @tparam T Type of stored elements
     */
    template <typename T>
    class ArrayList
    {
    public:

        static const uint32 MIN_INITIAL_SIZE = 16;

    public:

        /**
         * Creates an array with buffer size 'initialSize'
         * @param initialSize Initial size for Array list
         */
        explicit ArrayList(uint32 initialSize = MIN_INITIAL_SIZE);

        ~ArrayList();

        /**
         * Removes element from the list with calling default destructor (breaks the order)
         * @warning Assert on range check
         * @param index Element index to be deleted
         */
        void remove(uint32 index);

        /** Removes all the elements from list with calling default destructor */
        void empty();

        /**
         * Add elemnet in the end of the list
         * @warning if the expansion is locked and array is full it
         *          will raise a fatal error an shut down the engine
         * @param element To add
         */
        void operator +=(const T& element);

        /**
         * @warning Assert on range check
         * @param index Index of desired element
         * @return Element at index
         */
        T& operator [] (uint32 index);

        /** @return First element of array and reset iterator */
        T* iterate();

        /** @return Next element in the iteration */
        T* next();

        /** @return True is expansion is locked */
        bool isExpansionLocked() const;

        /** @return Current number of elements */
        uint32 getSize() const;

        /** @return Max number of elements */
        uint32 getCapacity() const;

        /** @return Expansion factor */
        float32 getExpansionFactor() const;

        /** @param lock Set in true to lock array expansion */
        void lockExpansion(bool lock);

    private:

        void expand();

    private:

        T*      mBuffer;
        uint32  mCurrent;
        uint32  mSize;
        uint32  mCapacity;
        bool    mLockExpansion;
        float32 mExpansionFactor;

    };

    template <typename T>
    ArrayList<T>::ArrayList(uint32 initialSize)
    {
        FAIL(initialSize >= MIN_INITIAL_SIZE, "Initial size must be more than %u", MIN_INITIAL_SIZE);

        mBuffer = nullptr;

        mSize = 0;
        mCurrent = 0;
        mCapacity = initialSize;
        mLockExpansion = false;
        mExpansionFactor = 1.5f;

        mBuffer = (T*) Allocator::getSingleton().memoryAllocate(mCapacity * sizeof(T));
    }

    template <typename T>
    ArrayList<T>::~ArrayList()
    {
        if (mBuffer)
        {
            PUSH("Array List: delete capacity: %u | buffer: %p", mCapacity, mBuffer);

            empty();
            Allocator::getSingleton().memoryFree(mBuffer);
            mBuffer = nullptr;
        }
    }

    template <typename T>
    void ArrayList<T>::remove(uint32 index)
    {
        FAIL(index < mSize, "Index out of range %u", index);

        mSize -= 1;
        mBuffer[index].~T();
        if (index == mSize) return;
        else memcpy(&mBuffer[index], &mBuffer[mSize], sizeof(T));
    }

    template <typename T>
    void ArrayList<T>::empty()
    {
        for (uint32 i = 0; i < mSize; i++)
        { mBuffer[i].~T(); }

        mSize = 0;
    }

    template <typename T>
    void ArrayList<T>::operator+=(const T& element)
    {
        if (mSize >= mCapacity)
        {
            if (mLockExpansion)
            {
                WARNING("Array List: Expansion of the internal buffer is locked %p", mBuffer);
                return;
            }
            else expand();
        }

        memcpy(&mBuffer[mSize++], &element, sizeof(T));
    }

    template <typename T>
    T& ArrayList<T>::operator[](uint32 index)
    {
        FAIL(index < mSize, "Index out of range %u", index);
        return mBuffer[index];
    }

    template <typename T>
    T* ArrayList<T>::iterate()
    {
        mCurrent = 0;
        return (mCurrent < mSize ? &(mBuffer[mCurrent]) : nullptr);
    }
    template <typename T>
    T* ArrayList<T>::next()
    {
        mCurrent += 1;
        return (mCurrent < mSize ? &(mBuffer[mCurrent]) : nullptr);
    }

    template <typename T>
    bool ArrayList<T>::isExpansionLocked() const
    {
        return mLockExpansion;
    }

    template <typename T>
    uint32 ArrayList<T>::getSize() const
    {
        return mSize;
    }

    template <typename T>
    uint32 ArrayList<T>::getCapacity() const
    {
        return mCapacity;
    }

    template <typename T>
    float32 ArrayList<T>::getExpansionFactor() const
    {
        return mExpansionFactor;
    }

    template <typename T>
    void ArrayList<T>::lockExpansion(bool lock)
    {
        mLockExpansion = lock;
    }

    template <typename T>
    void ArrayList<T>::expand()
    {
        mCapacity = (uint32)(mExpansionFactor * (float32)mCapacity);
        mBuffer = (T*) Allocator::getSingleton().memoryReallocate(mBuffer, mCapacity * sizeof(T));
    }

} // namespace Berserk

#endif //BERSERK_ARRAYLIST_H