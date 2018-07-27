//
// Created by Egor Orachyov on 27.07.2018.
//

#include "Containers/Queue.h"

namespace Berserk
{
    template <typename Element>
    Queue<Element>::Queue()
    {
        mIsLocked = false;
    }

    template <typename Element>
    Queue<Element>::~Queue()
    {
        empty();
    }

    template <typename Element>
    void Queue<Element>::init(UINT32 initialSize)
    {
        mIsLocked = false;
        mList.init(initialSize);
    }

    template <typename Element>
    void Queue<Element>::clean()
    {
        mList.clear();
    }

    template <typename Element>
    void Queue<Element>::empty()
    {
        mList.empty();
    }

    template <typename Element>
    void Queue<Element>::lock()
    {
        mIsLocked = true;
    }

    template <typename Element>
    void Queue<Element>::unlock()
    {
        mIsLocked = false;
    }

    template <typename Element>
    void Queue<Element>::add(const Element &element)
    {
        if (mIsLocked && mList.isFull())
        {
            return;
        }
        else
        {
            mList.add(element);
        }
    }

    template <typename Element>
    Element& Queue<Element>::get(UINT32 index) const
    {
        return mList.get(index);
    }

    template <typename Element>
    UINT32 Queue<Element>::getSize() const
    {
        return mList.getSize();
    }

    template <typename Element>
    UINT32 Queue<Element>::getCapacity() const
    {
        return mList.getCapacity();
    }

    template <typename Element>
    bool Queue<Element>::isLocked() const
    {
        return mIsLocked;
    }

} // namespace Berserk