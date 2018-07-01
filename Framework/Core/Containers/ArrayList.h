//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_ARRAYLIST_H
#define BERSERKENGINE_ARRAYLIST_H

#include "../Essential/Types.h"
#include "../Essential/Assert.h"

#include "../Memory/MemoryAllocators.h"

namespace Berserk
{

    /**
     *
     */
    template <typename Element> class ArrayList
    {
    public:

        /**
         *
         * @param initialSize
         */
        ArrayList(uint32 initialSize = 16);

        /**
         *
         */
        ~ArrayList();

        /**
         *
         * @param element
         */
        void Add(const Element& element);

        /**
         *
         * @param index
         */
        void Remove(uint32 index);

        /**
         *
         */
        void Empty();

        /**
         *
         * @param desiredCount
         */
        void Reserve(uint32 desiredCount);

        /**
         *
         * @return
         */
        uint32 GetSize() const;

        /**
         *
         * @return
         */
        uint32 GetCapacity() const;

        /**
         *
         * @return
         */
        uint32 GetElementSize() const;

        /**
         *
         * @param index
         * @return
         */
        Element& Get(uint32 index) const;

        /**
         *
         * @return
         */
        Element& GetFirst() const;

        /**
         *
         * @return
         */
        Element& GetLast() const;

        /**
         *
         * @return
         */
        Element* GetBuffer() const;

    private:

        /**
         *
         */
        void ExpandBuffer();

    private:

        uint32 mCapacity;           // Max number of elements in array without expansion
        uint32 mCurrentSize;        // Current number of element in the array
        Element* mBuffer;           // Pointer to buffer which stores elements in thr heap

    };

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
        mBuffer = 0;
        mCapacity = 0;
        mCurrentSize = 0;
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