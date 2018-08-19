//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_DOUBLELINKEDLIST_H
#define BERSERKENGINE_DOUBLELINKEDLIST_H

#include "Essential/Types.h"
#include "Essential/Assert.h"
#include "Essential/UsageDescriptors.h"

#include "Math/UtilityNumbers.h"
#include "Logging/LogMessages.h"

#include "Memory/PoolAllocator.h"

namespace Berserk
{

    /**
     * @addtogroup Containers
     *
     * @{
     */

    /**
     * Double linked list container based up on PoolAllocator. Store data (not pointers),
     * expand internal buffer if there is not enough memory
     */
    template <typename Element> class DATA_API DoubleLinkedList
    {
    private:

        /* List Node */
        struct Node
        {
        public:
            Element data;
            Node* next;
            Node* prev;
        };

    public:

        DoubleLinkedList();
        ~DoubleLinkedList();

        /**
         * Init list with start capacity
         * @param capacity Number of free elements in buffer
         */
        void init(UINT16 capacity = 16);

        /**
         * Add element to the list
         *
         * @param element Element which should be STORED into list
         */
        void add(const Element& element);

        /**
         * Add element in the front of the list
         *
         * @param element Element which should be STORED into list
         */
        void addFront(const Element& element);

        /**
         * Add element in the back of the list
         *
         * @param element Element which should be STORED into list
         */
        void addBack(const Element& element);

        /**
         * Removes element from the list if it exist
         *
         * @param element Element to be removed
         */
        void remove(const Element& element);

        /**
         * Delete first element from the list
         */
        void removeFirst();

        /**
         * Delete last element from the list
         */
        void removeLast();

        /**
         * Empty list and remove all the elements (will be called standard destructors
         * for each element)
         */
        void empty();

        /**
         * Get first element from the list
         *
         * @return First element
         */
        Element& getFirst() const;

        /**
         * Get current element of iterator
         *
         * @return Current element
         */
        Element& getCurrent() const;

        /**
         * Get last element of the list
         *
         * @return Last element
         */
        Element& getLast() const;

        /**
         * Returns the next element which is stored after param
         *
         * @param element Element to find the next after that
         * @return Element which stays after param element
         */
        Element& getNext(Element& element) const;

        /**
         * Iterator - return null if restart == false or returns
         * current element and moves iterator pointer to the next element
         *
         * @param restart Flag should the iterator begin from start
         * @return
         */
        Element* iterate(bool restart = false);

        /**
         * Get max num of elements which could be stored in the list
         *
         * @return Max num of element
         */
        UINT32 getCapacity() const;

        /**
         * Get num of elements currently stored in the list
         *
         * @return Current number of elements
         */
        UINT32 getSize() const;

        /**
         * Get total size of one list Node
         *
         * @return Size of Node
         */
        UINT32 getSizeOfNode() const;

    private:

        UINT32 mSize;
        Node* mHead;
        Node* mTail;
        Node* mIterator;
        PoolAllocator mPool;

    };

    /**
     * @}
     */

} // namespace Berserk

/// Implementation

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

#endif //BERSERKENGINE_DOUBLELINKEDLIST_H
