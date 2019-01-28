//
// Created by Egor Orachyov on 11.07.2018.
//

namespace Berserk
{

    template <typename Element>
    SharedList<Element>::SharedList()
    {
        mSize = 0;
        mHead = NULL;
        mTail = NULL;
        mIterator = NULL;
        mPool = NULL;
    }

    template <typename Element>
    SharedList<Element>::~SharedList()
    {
        // PUSH("Delete shared list %p shared allocator %p\n", this, mPool);
        empty();
    }

    template <typename Element>
    void SharedList<Element>::init(PoolAllocator *poolAllocator)
    {
        ASSERT(poolAllocator, "Pool allocator is NULL");
        ASSERT(!mHead, "List should have NULL head");
        ASSERT(!mTail, "List should have NULL head");

        mPool = poolAllocator;
        mSize = 0;
        mHead = NULL;
        mTail = NULL;
        mIterator = NULL;
    }

    template <typename Element>
    void SharedList<Element>::add(const Element& element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*)mPool->allocBlock();
            mHead->data = element;
            mHead->next = NULL;

            mTail = mHead;
        }
        else
        {
            mTail->next = (Node*)mPool->allocBlock();
            mTail = mTail->next;
            mTail->data = element;
            mTail->next = NULL;
        }

        mSize += 1;
    }

    template <typename Element>
    void SharedList<Element>::addFront(const Element& element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*)mPool->allocBlock();
            mHead->data = element;
            mHead->next = NULL;

            mTail = mHead;
        }
        else
        {
            Node* newHead = (Node*)mPool->allocBlock();
            newHead->next = mHead;
            newHead->data = element;
            mHead = newHead;
        }

        mSize += 1;
    }

    template <typename Element>
    void SharedList<Element>::addBack(const Element& element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*)mPool->allocBlock();
            mHead->data = element;
            mHead->next = NULL;

            mTail = mHead;
        }
        else
        {
            mTail->next = (Node*)mPool->allocBlock();
            mTail = mTail->next;
            mTail->data = element;
            mTail->next = NULL;
        }

        mSize += 1;
    }

    template <typename Element>
    void SharedList<Element>::remove(Element& element)
    {
        Node* prev = NULL;
        Node* tmp = mHead;
        while (tmp)
        {
            if (tmp->data == element)
            {
                if (!tmp->next)
                {
                    mTail = prev;
                }

                tmp->data.~Element();

                if (prev)
                {
                    prev->next = tmp->next;
                    mPool->freeBlock((void *) tmp);
                }
                else
                {
                    mHead = tmp->next;
                    mPool->freeBlock((void *) tmp);
                }

                mSize -= 1;
                mIterator = NULL;

                return;
            }

            prev = tmp;
            tmp = tmp->next;
        }
    }

    template <typename Element>
    void SharedList<Element>::empty()
    {
        Node* next;
        Node* tmp = mHead;
        while (tmp)
        {
            next = tmp->next;
            tmp->data.~Element();
            mPool->freeBlock((void *) tmp);
            tmp = next;
        }

        mSize = 0;
        mHead = NULL;
        mTail = NULL;
        mIterator = NULL;
    }

    template <typename Element>
    Element& SharedList<Element>::getFirst() const
    {
        ASSERT(mHead, "Data should be allocated before usage");
        return mHead->data;
    }

    template <typename Element>
    Element& SharedList<Element>::getLast() const
    {
        ASSERT(mTail, "Data should be allocated before usage");
        return mTail->data;
    }

    template <typename Element>
    Element* SharedList<Element>::iterate(bool restart)
    {
        if (restart)
        {
            mIterator = NULL;
        }
        else
        {
            if (mIterator == NULL)
            {
                mIterator = mHead;
            }
            else
            {
                mIterator = mIterator->next;
            }
        }

        if (mIterator == NULL)
        {
            return NULL;
        }
        else
        {
            return &mIterator->data;
        }
    }

    template <typename Element>
    Element& SharedList<Element>::getCurrent() const
    {
        return mIterator->data;
    }

    template <typename Element>
    Element& SharedList<Element>::getNext(Element& element) const
    {
        Node* tmp = mHead;
        while (tmp)
        {
            if (tmp->data == element)
            {
                return tmp->next->data;
            }

            tmp = tmp->next;
        }

        return NULL;
    }

    template <typename Element>
    UINT32 SharedList<Element>::getCapacity() const
    {
        return mPool->getCapacity();
    }

    template <typename Element>
    UINT32 SharedList<Element>::getSize() const
    {
        return mSize;
    }

    template <typename Element>
    UINT32 SharedList<Element>::getSizeOfNode() const
    {
        return sizeof(Node);
    }

} // namespace Berserk