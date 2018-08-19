//
// Created by Egor Orachyov on 11.07.2018.
//

namespace Berserk
{

    template <typename Element>
    LinkedList<Element>::LinkedList()
    {
        mSize = 0;
        mHead = NULL;
        mTail = NULL;
        mIterator = NULL;
    }

    template <typename Element>
    LinkedList<Element>::~LinkedList()
    {
        //PUSH("Delete linked list %p\n", this);

        empty();
        mPool.reset();
    }

    template <typename Element>
    void LinkedList<Element>::init(UINT16 capacity)
    {
        ASSERT(!mHead, "List should have NULL head");
        ASSERT(!mTail, "List should have NULL tail");

        mPool.init(sizeof(Node), capacity);
        mSize = 0;
        mHead = NULL;
        mTail = NULL;
        mIterator = NULL;
    }

    template <typename Element>
    void LinkedList<Element>::add(const Element& element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*)mPool.allocBlock();
            memcpy(&mHead->data, &element, sizeof(Element));
            //mHead->data = element;
            mHead->next = NULL;

            mTail = mHead;
        }
        else
        {
            mTail->next = (Node*)mPool.allocBlock();
            mTail = mTail->next;
            memcpy(&mTail->data, &element, sizeof(Element));
            //mTail->data = element;
            mTail->next = NULL;
        }

        mSize += 1;
    }

    template <typename Element>
    void LinkedList<Element>::addEmpty()
    {
        if (mHead == NULL)
        {
            mHead = (Node*)mPool.allocBlock();
            mHead->next = NULL;

            mTail = mHead;
        }
        else
        {
            mTail->next = (Node*)mPool.allocBlock();
            mTail = mTail->next;
            mTail->next = NULL;
        }

        mSize += 1;
    }

    template <typename Element>
    void LinkedList<Element>::addFront(const Element& element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*)mPool.allocBlock();
            memcpy(&mHead->data, &element, sizeof(Element));
            //mHead->data = element;
            mHead->next = NULL;

            mTail = mHead;
        }
        else
        {
            Node* newHead = (Node*)mPool.allocBlock();
            newHead->next = mHead;
            memcpy(&newHead->data, &element, sizeof(Element));
            //newHead->data = element;
            mHead = newHead;
        }

        mSize += 1;
    }

    template <typename Element>
    void LinkedList<Element>::addBack(const Element& element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*)mPool.allocBlock();
            memcpy(&mHead->data, &element, sizeof(Element));
            //mHead->data = element;
            mHead->next = NULL;

            mTail = mHead;
        }
        else
        {
            mTail->next = (Node*)mPool.allocBlock();
            mTail = mTail->next;
            memcpy(&mTail->data, &element, sizeof(Element));
            //mTail->data = element;
            mTail->next = NULL;
        }

        mSize += 1;
    }

    template <typename Element>
    void LinkedList<Element>::remove(Element& element)
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
    void LinkedList<Element>::empty()
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
    Element& LinkedList<Element>::get(UINT32 i) const
    {
        if (mSize == 0)
        {
            ERROR("LinkedList: Attempt to get element via index in empty list");
            return mHead->data;
        }

        UINT32 index = i % mSize;
        i = 0;
        Node* tmp = mHead;

        while (i < index)
        {
            tmp = tmp->next;
            i += 1;
        }

        return tmp->data;
    }

    template <typename Element>
    Element& LinkedList<Element>::getFirst() const
    {
        ASSERT(mHead, "Data should be allocated before usage");
        return mHead->data;
    }

    template <typename Element>
    Element& LinkedList<Element>::getLast() const
    {
        ASSERT(mTail, "Data should be allocated before usage");
        return mTail->data;
    }

    template <typename Element>
    Element* LinkedList<Element>::iterate(bool restart)
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
    Element& LinkedList<Element>::getCurrent() const
    {
        return mIterator->data;
    }

    template <typename Element>
    Element& LinkedList<Element>::getNext(Element& element) const
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
    UINT32 LinkedList<Element>::getCapacity() const
    {
        return mPool.getCapacity();
    }

    template <typename Element>
    UINT32 LinkedList<Element>::getSize() const
    {
        return mSize;
    }

    template <typename Element>
    UINT32 LinkedList<Element>::getSizeOfNode() const
    {
        return sizeof(Node);
    }

} // namespace Berserk