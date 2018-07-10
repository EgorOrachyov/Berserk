//
// Created by Egor Orachyov on 11.07.2018.
//

namespace Berserk
{

    template <typename Element>
    ArrayList<Element>::ArrayList()
    {
        mCurrentSize = 0;
        mCapacity = 0;
        mBuffer = NULL;
    }

    template <typename Element>
    ArrayList<Element>::~ArrayList()
    {
        PUSH("Delete array list %p with buffer %p\n", this, mBuffer);

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
    void ArrayList<Element>::empty()
    {
        for(INT32 i = 0; i < mCurrentSize; i++)
        {
            mBuffer[i].~Element();
        }

        if (mBuffer) {
            mem_free(mBuffer);
        }
        mBuffer = NULL;
        mCapacity = 0;
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
        return &mBuffer;
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

} // namespace Berserk