//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_ARRAYLIST_H
#define BERSERK_ARRAYLIST_H

#include "Misc/Types.h"
#include "Misc/Assert.h"
#include "Object/NewDelete.h"
#include "Memory/Allocator.h"
#include "Memory/IAllocator.h"
#include "Logging/LogMacros.h"
#include "Profiling/ProfilingMacro.h"

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

        static const uint32 MIN_INITIAL_SIZE = 2;

        static const uint32 DEFAULT_INITIAL_SIZE = 16;


    public:

        ArrayList() : mBuffer(nullptr),
                      mCurrent(0),
                      mSize(0),
                      mCapacity(0),
                      mLockExpansion(false),
                      mExpansionFactor(0),
                      mAllocator(nullptr)
        {

        };

        GEN_NEW_DELETE(ArrayList);

    public:

        /**
         * Creates an array with buffer size 'initialSize'
         * @param initialSize Initial size for Array list
         */
        explicit ArrayList(uint32 initialSize, IAllocator* allocator = nullptr);

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

        /** @return Pointer to internal buffer */
        T* get() { return mBuffer; }

        /** @return Current number of elements */
        uint32 getSize() const;

        /** @return Max number of elements */
        uint32 getCapacity() const;

        /** @return Memory cost of this resource (on CPU side only) */
        uint32 getMemoryUsage() const;

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
        IAllocator* mAllocator;

    };

    template <typename T>
    ArrayList<T>::ArrayList(uint32 initialSize, IAllocator* allocator)
    {
        FAIL(initialSize >= MIN_INITIAL_SIZE, "Initial size must be more than %u", MIN_INITIAL_SIZE);

        mBuffer = nullptr;

        mSize = 0;
        mCurrent = 0;
        mCapacity = initialSize;
        mLockExpansion = false;
        mExpansionFactor = 1.5f;

        if (allocator) mAllocator = allocator;
        else mAllocator = &Allocator::getSingleton();

        mBuffer = (T*) mAllocator->allocate(mCapacity * sizeof(T));
    }

    template <typename T>
    ArrayList<T>::~ArrayList()
    {
        if (mBuffer)
        {
#if PROFILE_ARRAY_LIST
            PUSH("Array List: delete capacity: %u | buffer: %p", mCapacity, mBuffer);
#endif

            empty();
            mAllocator->free(mBuffer);
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

            expand();
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
    uint32 ArrayList<T>::getMemoryUsage() const
    {
        return mCapacity * sizeof(T);
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
        // Save old pointer and its size to
        // copy data into new allocated buffer of bigger size

        auto old = mBuffer;
        auto size = mCapacity * sizeof(T);

        mCapacity = (uint32)(mExpansionFactor * (float32)mCapacity);

        // Imitation of realloc method for IAllocator

        mBuffer = (T*) mAllocator->allocate(mCapacity * sizeof(T));
        memcpy(mBuffer, old, size);
        mAllocator->free(old);
    }

} // namespace Berserk

#endif //BERSERK_ARRAYLIST_H