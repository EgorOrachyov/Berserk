//
// Created by Egor Orachyov on 27.07.2018.
//

#include "Containers/IList.h"

namespace Berserk
{
    template <typename Element>
    List<Element>::List()
    {
        mIsLocked = false;
    }

    template <typename Element>
    List<Element>::~List()
    {
        empty();
    }

    template <typename Element>
    void List<Element>::init(UINT32 initialSize)
    {
        mIsLocked = false;
        mList.init(initialSize);
    }

    template <typename Element>
    void List<Element>::clean()
    {
        mList.clear();
    }

    template <typename Element>
    void List<Element>::empty()
    {
        mList.empty();
    }

    template <typename Element>
    void List<Element>::lock()
    {
        mIsLocked = true;
    }

    template <typename Element>
    void List<Element>::unlock()
    {
        mIsLocked = false;
    }

    template <typename Element>
    void List<Element>::add(const Element &element)
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
    Element& List<Element>::get(UINT32 index) const
    {
        return mList.get(index);
    }

    template <typename Element>
    Element& List<Element>::getLast() const
    {
        return mList.getLast();
    }

    template <typename Element>
    UINT32 List<Element>::getSize() const
    {
        return mList.getSize();
    }

    template <typename Element>
    UINT32 List<Element>::getCapacity() const
    {
        return mList.getCapacity();
    }

    template <typename Element>
    bool List<Element>::isLocked() const
    {
        return mIsLocked;
    }

} // namespace Berserk