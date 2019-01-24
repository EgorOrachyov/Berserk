//
// Created by Egor Orachyov on 11.07.2018.
//

namespace Berserk
{

    template <typename Element>
    DoubleLinkedList<Element>::DoubleLinkedList()
    {
        mSize = 0;
        mHead = NULL;
        mTail = NULL;
        mIterator = NULL;
    }

    template <typename Element>
    DoubleLinkedList<Element>::~DoubleLinkedList()
    {
        //PUSH("Delete double linked list %p\n", this);

        empty();
        mPool.reset();
    }

    template <typename Element>
    void DoubleLinkedList<Element>::init(UINT16 capacity)
    {
        ASSERT(!mHead, "List should have NULL head");
        ASSERT(!mTail, "List should have NULL head");

        mPool.init(sizeof(Node), capacity);
        mSize = 0;
        mHead = NULL;
        mTail = NULL;
        mIterator = NULL;
    }

    template <typename Element>
    void DoubleLinkedList<Element>::add(const Element& element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*)mPool.allocBlock();
            mHead->data = element;
            mHead->next = NULL;
            mHead->prev = NULL;

            mTail = mHead;
        }
        else
        {
            printf("%i \n", getSize());

            mTail->next = (Node*)mPool.allocBlock();
            mTail->next->prev = mTail;
            mTail = mTail->next;
            mTail->data = element;
            mTail->next = NULL;
        }

        mSize += 1;
    }

    template <typename Element>
    void DoubleLinkedList<Element>::addFront(const Element& element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*)mPool.allocBlock();
            mHead->data = element;
            mHead->next = NULL;
            mHead->prev =  NULL;

            mTail = mHead;
        }
        else
        {
            Node* newHead = (Node*)mPool.allocBlock();
            mHead->prev = newHead;
            newHead->next = mHead;
            newHead->prev = NULL;
            newHead->data = element;
            mHead = newHead;
        }

        mSize += 1;
    }

    template <typename Element>
    void DoubleLinkedList<Element>::addBack(const Element& element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*)mPool.allocBlock();
            mHead->data = element;
            mHead->next = NULL;
            mHead->prev = NULL;

            mTail = mHead;
        }
        else
        {
            mTail->next = (Node*)mPool.allocBlock();
            mTail->next->prev = mTail;
            mTail = mTail->next;
            mTail->data = element;
            mTail->next = NULL;
        }

        mSize += 1;
    }

    template <typename Element>
    void DoubleLinkedList<Element>::remove(const Element& element)
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
                    mPool.freeBlock((void*)tmp);
                }
                else
                {
                    mHead = tmp->next;
                    mPool.freeBlock((void*)tmp);
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
    void DoubleLinkedList<Element>::removeFirst()
    {
        if (mHead)
        {
            mHead->data.~Element();
            Node* tmp = mHead;
            mHead = tmp->next;
            mHead->prev = NULL;
            mPool.freeBlock((void*)tmp);

            mSize -= 1;
            mIterator = NULL;
        }
    }

    template <typename Element>
    void DoubleLinkedList<Element>::removeLast()
    {
        if (mTail)
        {
            mTail->data.~Element();
            Node* tmp = mTail;
            mTail = tmp->prev;
            mTail->next = NULL;
            mPool.freeBlock((void*)tmp);

            mSize -= 1;
            mIterator = NULL;
        }
    }

    template <typename Element>
    void DoubleLinkedList<Element>::empty()
    {
        Node* next;
        Node* tmp = mHead;
        while (tmp)
        {
            next = tmp->next;
            tmp->data.~Element();
            mPool.freeBlock((void*)tmp);
            tmp = next;
        }

        mSize = 0;
        mHead = NULL;
        mTail = NULL;
        mIterator = NULL;
    }

    template <typename Element>
    Element& DoubleLinkedList<Element>::getFirst() const
    {
        ASSERT(mHead, "Data should be allocated before usage");
        return mHead->data;
    }

    template <typename Element>
    Element& DoubleLinkedList<Element>::getLast() const
    {
        ASSERT(mTail, "Data should be allocated before usage");
        return mTail->data;
    }

    template <typename Element>
    Element* DoubleLinkedList<Element>::iterate(bool restart)
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
    Element& DoubleLinkedList<Element>::getCurrent() const
    {
        return mIterator->data;
    }

    template <typename Element>
    Element& DoubleLinkedList<Element>::getNext(Element& element) const
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
    UINT32 DoubleLinkedList<Element>::getCapacity() const
    {
        return mPool.getCapacity();
    }

    template <typename Element>
    UINT32 DoubleLinkedList<Element>::getSize() const
    {
        return mSize;
    }

    template <typename Element>
    UINT32 DoubleLinkedList<Element>::getSizeOfNode() const
    {
        return sizeof(Node);
    }

} // namespace Berserk