//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_ARRAYLIST_H
#define BERSERK_ARRAYLIST_H

#include "Public/Misc/Types.h"
#include "Public/Misc/Assert.h"
#include "Public/Memory/Allocator.h"

namespace Berserk
{

    /**
     * Dynamically expandable array list for elements of type T.
     * Automatically expands in the add method whether it does not have enough
     * space in the internal buffer. Relies on default platform allocator.
     * Provides iteration mechanism for elements for using in for loop.
     *
     * @tparam T Type of stored elements
     * @tparam size Initial size for Array list
     */
    template <typename T, uint32 size = 64>
    class ArrayList
    {
    public:

        static const uint32 MIN_INITIAL_SIZE = 16;

    public:

        /** Creates an array with buffer size 'size' */
        ArrayList();

        ~ArrayList();

        /**
         * Removes element from the list with calling default destructor (breaks the order)
         * @warning Assert on range check
         * @param index Element index to be deleted
         */
        void remove(uint32 index);

        /** Removes all the elements from list with calling default destructor */
        void removeAll();

        /**
         * Add elemnet in the end of the list
         * @warning if the expansion is locked and array is full it
         *          will raise a fatal error an shut down the engine
         * @param element To add
         */
        void operator += (T element);

        /**
         * @warning Assert on range check
         * @param index Index of desired element
         * @return Element at index
         */
        T operator [] (uint32 index);

        /** @return First element of array and reset iterator */
        T iterate();

        /** @return Next element in the iteration */
        T getNext();

        /** If iteration is ended (has no element) */
        bool end();

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

    template <typename T, uint32 size>
    ArrayList<T,size>::ArrayList()
    {
        FATAL(size >= MIN_INITIAL_SIZE, "Initial size must be more than %u", MIN_INITIAL_SIZE);

        mBuffer = nullptr;

        mSize = 0;
        mCurrent = 0;
        mCapacity = size;
        mLockExpansion = false;
        mExpansionFactor = 1.5f;

        mBuffer = (T*) Allocator::getSingleton().memoryAllocate(mCapacity * sizeof(T));
    }

    template <typename T, uint32 size>
    ArrayList<T,size>::~ArrayList()
    {
        if (mBuffer)
        {
            removeAll();
            Allocator::getSingleton().memoryFree(mBuffer);
            mBuffer = nullptr;

            fprintf(stdout, "Array List: delete buffer %u\n", mCapacity);
        }
    }

    template <typename T, uint32 size>
    void ArrayList<T,size>::remove(uint32 index)
    {
        FATAL(index < mSize, "Index out of range %u", index);

        mSize -= 1;
        mBuffer[index].~T();
        if (index == mSize) return;
        else memcpy(&mBuffer[index], &mBuffer[mSize], sizeof(T));
    }

    template <typename T, uint32 size>
    void ArrayList<T,size>::removeAll()
    {
        for (uint32 i = 0; i < mSize; i++)
        { mBuffer[i].~T(); }

        mSize = 0;
    }

    template <typename T, uint32 size>
    void ArrayList<T,size>::operator+=(T element)
    {
        // todo: Log Message employment

        if (mSize >= mCapacity)
            if (mLockExpansion) fprintf(stderr, "Array List: Expansion of the internal buffer is locked\n");
            else expand();
        memcpy(&mBuffer[mSize++], &element, sizeof(T));
    }

    template <typename T, uint32 size>
    T ArrayList<T,size>::operator[](uint32 index)
    {
        FATAL(index < mSize, "Index out of range %u", index);
        return mBuffer[index];
    }

    template <typename T, uint32 size>
    T ArrayList<T,size>::iterate()
    {
        mCurrent = 0;
        return mBuffer[0];
    }
    template <typename T, uint32 size>
    T ArrayList<T,size>::getNext()
    {
        mCurrent += 1;
        return mBuffer[mCurrent];
    }

    template <typename T, uint32 size>
    bool ArrayList<T,size>::end()
    {
        return (mCurrent >= mSize);
    }

    template <typename T, uint32 size>
    bool ArrayList<T,size>::isExpansionLocked() const
    {
        return mLockExpansion;
    }

    template <typename T, uint32 size>
    uint32 ArrayList<T,size>::getSize() const
    {
        return mSize;
    }

    template <typename T, uint32 size>
    uint32 ArrayList<T,size>::getCapacity() const
    {
        return mCapacity;
    }

    template <typename T, uint32 size>
    float32 ArrayList<T,size>::getExpansionFactor() const
    {
        return mExpansionFactor;
    }

    template <typename T, uint32 size>
    void ArrayList<T,size>::lockExpansion(bool lock)
    {
        mLockExpansion = lock;
    }

    template <typename T, uint32 size>
    void ArrayList<T,size>::expand()
    {
        mCapacity = (uint32)(mExpansionFactor * (float32)mCapacity);
        mBuffer = (T*) Allocator::getSingleton().memoryReallocate(mBuffer, mCapacity * sizeof(T));
    }

} // namespace Berserk

#endif //BERSERK_ARRAYLIST_H