//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_ARRAYLIST_H
#define BERSERKENGINE_ARRAYLIST_H

#include "../Essential/Types.h"
#include "../Essential/Assert.h"
#include "../Essential/UsageDescriptors.h"

#include "../Memory/MemoryAllocators.h"

namespace Berserk
{

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
         * Initialize list with desired start size
         * @param initialSize
         */
        ArrayList(uint32 initialSize);

        /**
         * Calls empty
         */
        ~ArrayList();

        /**
         * Initialize list with desired start size
         * @param initialSize (default value 16)
         */
        void Init(uint32 initialSize = 16);

        /**
         * Add new element to the list
         *
         * @note Element will be stored in the list by value, not by reference
         *
         * @param element
         */
        void Add(const Element& element);

        /**
         * Remove element with index
         * @param index
         */
        void Remove(uint32 index);

        /**
         * Calls destructors for all the elements, frees buffer's data
         */
        void Empty();

        /**
         * Reserve desired number of elements
         *
         * @warning Should be more than 16
         * @warning If current number of elements more than desiredCount they will be destroyed
         * and deleted from the list data
         *
         * @param desiredCount
         */
        void Reserve(uint32 desiredCount);

        /**
         * Number of elements in the list
         * @return Current count
         */
        uint32 GetSize() const;

        /**
         * Max possible umber of elements in the list
         * @return Max count
         */
        uint32 GetCapacity() const;

        /**
         * Size of the element in the bytes
         * @return Size in bytes
         */
        uint32 GetElementSize() const;

        /**
         * Get element with index
         *
         * @warning Index should be in [0;size)
         *
         * @param index
         * @return Elemnet
         */
        Element& Get(uint32 index) const;

        /**
         * Get first element
         *
         * @warning List should not be empty
         *
         * @return Element
         */
        Element& GetFirst() const;

        /**
         * Get last element
         *
         * @warning List should not be empty
         *
         * @return Element
         */
        Element& GetLast() const;

        /**
         * Get pointer to internal elements buffer
         *
         * @return Pointer to data
         */
        Element* GetBuffer() const;

    private:

        /**
         * Will expand buffer by 2 if it is need
         */
        void ExpandBuffer();

    private:

        uint32 mCapacity;           // Max number of elements in array without expansion
        uint32 mCurrentSize;        // Current number of element in the array
        Element* mBuffer;           // Pointer to buffer which stores elements in thr heap

    };

    template <typename Element>
    ArrayList<Element>::ArrayList()
    {
        mCurrentSize = 0;
        mCapacity = 0;
        mBuffer = NULL;
    }

    template <typename Element>
    ArrayList<Element>::ArrayList(uint32 initialSize)
    {
        ASSERT(initialSize > 0, "Initial size should be more than 0");

        mCurrentSize = 0;
        mCapacity = initialSize;
        mBuffer = (Element*)mem_alloc(initialSize * sizeof(Element));
    }

    template <typename Element>
    ArrayList<Element>::~ArrayList()
    {
        Empty();
    }

    template <typename Element>
    void ArrayList<Element>::Init(uint32 initialSize)
    {
        ASSERT(initialSize > 0, "Initial size should be more than 0");

        mCurrentSize = 0;
        mCapacity = initialSize;
        mBuffer = (Element*)mem_alloc(initialSize * sizeof(Element));
    }

    template <typename Element>
    void ArrayList<Element>::Add(const Element& element)
    {
        ExpandBuffer();
        mBuffer[mCurrentSize++] = element;
    }

    template <typename Element>
    void ArrayList<Element>::Remove(uint32 index)
    {
        ASSERT(index < mCurrentSize, "Index should be in [0;size)");

        mCurrentSize -= 1;
        mBuffer[index].~Element();
        for(int32 i = index; i < mCurrentSize; i++)
        {
            mBuffer[i] = mBuffer[i + 1];
        }
    }

    template <typename Element>
    void ArrayList<Element>::Empty()
    {
        for(int32 i = 0; i < mCurrentSize; i++)
        {
            mBuffer[i].~Element();
        }

        mem_free(mBuffer);
        mBuffer = NULL;
        mCapacity = 0;
        mCurrentSize = 0;
    }

    template <typename Element>
    void ArrayList<Element>::Reserve(uint32 desiredCount)
    {
        if (desiredCount > 1)
        {
            mCapacity = desiredCount; // expand factor 2
            mBuffer = (Element*)mem_realloc(mBuffer, mCapacity * sizeof(Element));
        }
    }

    template <typename Element>
    uint32 ArrayList<Element>::GetSize() const
    {
        return mCurrentSize;
    }

    template <typename Element>
    uint32 ArrayList<Element>::GetCapacity() const
    {
        return mCapacity;
    }

    template <typename Element>
    uint32 ArrayList<Element>::GetElementSize() const
    {
        return sizeof(Element);
    }

    template <typename Element>
    Element& ArrayList<Element>::Get(uint32 index) const
    {
        ASSERT(index < mCurrentSize, "Index should be in [0;size)");

        return mBuffer[index];
    }

    template <typename Element>
    Element& ArrayList<Element>::GetFirst() const
    {
        ASSERT(mCurrentSize > 0, "Array list is empty");

        return mBuffer[0];
    }

    template <typename Element>
    Element& ArrayList<Element>::GetLast() const
    {
        ASSERT(mCurrentSize > 0, "Array list is empty");
        return mBuffer[mCurrentSize - 1];
    }

    template <typename Element>
    Element* ArrayList<Element>::GetBuffer() const
    {
        return &mBuffer;
    }

    template <typename Element>
    void ArrayList<Element>::ExpandBuffer()
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

} // namespace Berserk

#endif //BERSERKENGINE_ARRAYLIST_H