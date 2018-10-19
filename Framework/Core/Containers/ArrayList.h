//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_ARRAYLIST_H
#define BERSERKENGINE_ARRAYLIST_H

#include "Misc/Types.h"
#include "Misc/Assert.h"
#include "Misc/UsageDescriptors.h"

#include "Logging/LogMessages.h"
#include "Memory/MemoryAllocators.h"

namespace Berserk
{
    /**
     * @defgroup Containers
     * @brief Data structures
     *
     * Module provides common data structures for engine sub-systems. Note that
     * containers are oriented on STORING data inside that (not handling a pointer)
     *
     * @ingroup Core
     *
     * @{
     */

    /**
     * Dynamic array for elements which allocates memory in the heap if its is not
     * enough for next pushing of element. Should be used for elements with default
     * destructors for simple storing of that in this container.
     *
     * @note If you want to store huge data with complex destructors look at the
     * Linked List container
     */
    template <typename Element> class DATA_API ArrayList
    {
    public:

        /**
         * Initialize empty array list
         */
        ArrayList();

        /**
         * Calls empty
         */
        ~ArrayList();

        /**
         * Initialize list with desired start size
         * @param initialSize (default value 16)
         */
        void init(UINT32 initialSize = 16);

        /**
         * Add new element to the list
         *
         * @note Element will be stored in the list by value, not by reference
         *
         * @param element
         */
        void add(const Element &element);

        /**
         * Remove element with index
         * @param index
         */
        void remove(UINT32 index);

        /**
         * Remove element with index ignore order of elements
         * @param index
         */
        void removeIgnoreOrder(UINT32 index);

        /**
         * Calls destructors for all the elements, frees buffer's data
         */
        void empty();

        /**
         * Empty array without calling destructors for each element (O(1))
         * and DO NOT FREE internal data
         */
        void clear();

        /**
         * Reserve desired number of elements
         *
         * @warning Should be more than 16
         * @warning If current number of elements more than desiredCount they will be destroyed
         * and deleted from the list data
         *
         * @param desiredCount
         */
        void reserve(UINT32 desiredCount);

        /**
         * Number of elements in the list
         * @return Current count
         */
        UINT32 getSize() const;

        /**
         * Max possible umber of elements in the list
         * @return Max count
         */
        UINT32 getCapacity() const;

        /**
         * Size of the element in the bytes
         * @return Size in bytes
         */
        UINT32 getElementSize() const;

        /**
         * Get element with index like a pointer
         *
         * @param index
         * @return Pointer to element
         */
        Element* getPointer(UINT32 index) const;

        /**
         * Get element with index
         *
         * @warning Index should be in [0;size)
         *
         * @param index
         * @return Elemnet
         */
        Element& get(UINT32 index) const;

        /**
         * Get first element
         *
         * @warning List should not be empty
         *
         * @return Element
         */
        Element& getFirst() const;

        /**
         * Get last element
         *
         * @warning List should not be empty
         *
         * @return Element
         */
        Element& getLast() const;

        /**
         * Get pointer to internal elements buffer
         *
         * @return Pointer to data
         */
        Element* getBuffer() const;

        /**
         * Is buffer full (cannot add without expansion)
         *
         * @return true if size == capacity
         */
        bool isFull() const;

    private:

        /**
         * Will expand buffer by 2 if it is need
         */
        void expandBuffer();

    private:

        UINT32 mCapacity;           // Max number of elements in array without expansion
        UINT32 mCurrentSize;        // Current number of element in the array
        Element* mBuffer;           // Pointer to buffer which stores elements in the heap

    };

    /**
     * @}
     */

} // namespace Berserk

/// Implementation

namespace Berserk
{

    template <typename Element>
    ArrayList<Element>::ArrayList()
    {
        mCurrentSize = 0;
        mCapacity = 0;
        mBuffer = nullptr;
    }

    template <typename Element>
    ArrayList<Element>::~ArrayList()
    {
        //PUSH("Delete array list %p with buffer %p\n", this, mBuffer);

        empty();
    }

    template <typename Element>
    void ArrayList<Element>::init(UINT32 initialSize)
    {
        ASSERT(initialSize > 0, "Initial size should be more than 0");

        mCurrentSize = 0;
        mCapacity = initialSize;
        mBuffer = (Element*)mem_alloc(initialSize * sizeof(Element));
    }

    template <typename Element>
    void ArrayList<Element>::add(const Element &element)
    {
        expandBuffer();
        mBuffer[mCurrentSize++] = element;
    }

    template <typename Element>
    void ArrayList<Element>::remove(UINT32 index)
    {
        ASSERT(index < mCurrentSize, "Index should be in [0;size)");

        mCurrentSize -= 1;
        mBuffer[index].~Element();
        for(INT32 i = index; i < mCurrentSize; i++)
        {
            mBuffer[i] = mBuffer[i + 1];
        }
    }

    template <typename Element>
    void ArrayList<Element>::removeIgnoreOrder(UINT32 index)
    {
        ASSERT(index < mCurrentSize, "Index should be in [0;size)");

        mCurrentSize -= 1;
        mBuffer[index].~Element();

        if (index != mCurrentSize)
        {
            mBuffer[index] = mBuffer[mCurrentSize];
        }
    }

    template <typename Element>
    void ArrayList<Element>::empty()
    {
        for(INT32 i = 0; i < mCurrentSize; i++)
        {
            mBuffer[i].~Element();
        }

        if (mBuffer)
        {
            mem_free(mBuffer);
        }

        mBuffer = nullptr;
        mCapacity = 0;
        mCurrentSize = 0;
    }

    template <typename Element>
    void ArrayList<Element>::clear()
    {
        mCurrentSize = 0;
    }

    template <typename Element>
    void ArrayList<Element>::reserve(UINT32 desiredCount)
    {
        if (desiredCount > 1)
        {
            mCapacity = desiredCount; // expand factor 2
            mBuffer = (Element*)mem_realloc(mBuffer, mCapacity * sizeof(Element));
        }
    }

    template <typename Element>
    UINT32 ArrayList<Element>::getSize() const
    {
        return mCurrentSize;
    }

    template <typename Element>
    UINT32 ArrayList<Element>::getCapacity() const
    {
        return mCapacity;
    }

    template <typename Element>
    UINT32 ArrayList<Element>::getElementSize() const
    {
        return sizeof(Element);
    }

    template <typename Element>
    Element* ArrayList<Element>::getPointer(UINT32 index) const
    {
        ASSERT(index < mCurrentSize, "Index should be in [0;size)");

        return &mBuffer[index];
    }

    template <typename Element>
    Element& ArrayList<Element>::get(UINT32 index) const
    {
        ASSERT(index < mCurrentSize, "Index should be in [0;size)");

        return mBuffer[index];
    }

    template <typename Element>
    Element& ArrayList<Element>::getFirst() const
    {
        ASSERT(mCurrentSize > 0, "Array list is empty");

        return mBuffer[0];
    }

    template <typename Element>
    Element& ArrayList<Element>::getLast() const
    {
        ASSERT(mCurrentSize > 0, "Array list is empty");
        return mBuffer[mCurrentSize - 1];
    }

    template <typename Element>
    Element* ArrayList<Element>::getBuffer() const
    {
        return mBuffer;
    }

    template <typename Element>
    void ArrayList<Element>::expandBuffer()
    {
        if (mBuffer == NULL)
        {
            mCurrentSize = 0;
            mCapacity = 16;
            mBuffer = (Element*)mem_alloc(mCapacity * sizeof(Element));
        }
        else if (mCurrentSize >= mCapacity)
        {
            mCapacity *= 2; // expand factor 2
            mBuffer = (Element*)mem_realloc(mBuffer, mCapacity * sizeof(Element));
        }
    }

    template <typename Element>
    bool ArrayList<Element>::isFull() const
    {
        return (mCurrentSize == mCapacity);
    }

} // namespace Berserk

#endif //BERSERKENGINE_ARRAYLIST_H