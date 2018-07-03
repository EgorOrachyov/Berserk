//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_LINKEDLIST_H
#define BERSERKENGINE_LINKEDLIST_H

#include "../Essential/Types.h"
#include "../Essential/Assert.h"
#include "../Essential/UsageDescriptors.h"

#include "Memory/PoolAllocator.h"

namespace Berserk
{

    /**
     * Linked list container based up on PoolAllocator. It means that
     * elements will be stored in the internal pre-allocated buffer and
     * list will have fixed capacity, which could not be changed after initializing
     */
    template <typename Element> class DATA_API LinkedList
    {
    private:

        /* List Node */
        typedef struct Node
        {
        public:
            Element data;
            Node* next;
        } Node;

    public:

        LinkedList();
        ~LinkedList();

        /**
         * Initializing of linked list (should be called before use if or not
         * was used standard constructor)
         *
         * @param poolAllocator Pointer to allocator which will store list's nodes
         */
        void init(PoolAllocator *poolAllocator);

        /**
         * Add element to the list
         *
         * @param element Element which should be STORED into list
         */
        void add(Element element);

        /**
         * Add element to the list
         *
         * @param element Element which should be STORED into list
         */
        void add(Element *element);

        /**
         * Add element in the front of the list
         *
         * @param element Element which should be STORED into list
         */
        void addFront(Element element);

        /**
         * Add element in the back of the list
         *
         * @param element Element which should be STORED into list
         */
        void addBack(Element element);

        /**
         * Removes element from the list if it existsx
         *
         * @param element Element to be removed
         */
        void remove(Element element);

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
        Element getFirst();

        /**
         * Get current element of iterator
         *
         * @return Current element
         */
        Element getCurrent();

        /**
         * Get last element of the list
         *
         * @return Last element
         */
        Element getLast();

        /**
         * Returns the next element which is stored after param
         *
         * @param element Element to find the next after that
         * @return Element which stays after param element
         */
        Element getNext(Element element);

        /**
         * Iterator - return null if restart == false or returns
         * current element and moves iterator pointer to the next element
         *
         * @param restart Flag should the iterator begin from start
         * @return
         */
        Element iterate(bool restart = false);

        /**
         * Get max num of elements which could be stored in the list
         *
         * @return Max num of element
         */
        uint32 getCapacity();

        /**
         * Get num of elements currently stored in the list
         *
         * @return Current number of elements
         */
        uint32 getSize();

        /**
         * Get total size of one list Node
         *
         * @return Size of Node
         */
        uint32 getSizeOfNode();

    private:

        uint32 mSize;
        Node* mHead;
        Node* mTail;
        Node* mIterator;
        PoolAllocator* mPool;

    };

    template <typename Element>
    LinkedList<Element>::LinkedList()
    {
        mSize = 0;
        mHead = NULL;
        mTail = NULL;
        mIterator = NULL;
        mPool = NULL;
    }

    template <typename Element>
    LinkedList<Element>::~LinkedList()
    {
        empty();
    }

    template <typename Element>
    void LinkedList<Element>::init(PoolAllocator *poolAllocator)
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
    void LinkedList<Element>::add(Element element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*) mPool->allocBlock();
            mHead->data = element;
            mHead->next = NULL;

            mTail = mHead;
        }
        else
        {
            mTail->next = (Node*) mPool->allocBlock();
            mTail = mTail->next;
            mTail->data = element;
            mTail->next = NULL;
        }

        mSize += 1;
    }

    template <typename Element>
    void LinkedList<Element>::add(Element *element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*) mPool->allocBlock();
            memcpy(&mHead->data, element, sizeof(Element));
            mHead->next = NULL;

            mTail = mHead;
        }
        else
        {
            mTail->next = (Node*) mPool->allocBlock();
            mTail = mTail->next;
            memcpy(&mTail->data, element, sizeof(Element));
            mTail->next = NULL;
        }

        mSize += 1;
    }

    template <typename Element>
    void LinkedList<Element>::addFront(const Element element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*) mPool->allocBlock();
            mHead->data = element;
            mHead->next = NULL;

            mTail = mHead;
        }
        else
        {
            Node* newHead = (Node*) mPool->allocBlock();
            newHead->next = mHead;
            newHead->data = element;
            mHead = newHead;
        }

        mSize += 1;
    }

    template <typename Element>
    void LinkedList<Element>::addBack(const Element element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*) mPool->allocBlock();
            mHead->data = element;
            mHead->next = NULL;

            mTail = mHead;
        }
        else
        {
            mTail->next = (Node*) mPool->allocBlock();
            mTail = mTail->next;
            mTail->data = element;
            mTail->next = NULL;
        }

        mSize += 1;
    }

    template <typename Element>
    void LinkedList<Element>::remove(const Element element)
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
    void LinkedList<Element>::empty()
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
    Element LinkedList<Element>::getFirst()
    {
        ASSERT(mHead, "Data should be allocated before usage");
        return mHead->data;
    }

    template <typename Element>
    Element LinkedList<Element>::getLast()
    {
        ASSERT(mTail, "Data should be allocated before usage");
        return mTail->data;
    }

    template <typename Element>
    Element LinkedList<Element>::iterate(bool restart)
    {
        Element element;

        if (restart)
        {
            element = NULL;
            mIterator = mHead;
        }
        else
        {
            if (mIterator)
            {
                element = mIterator->data;
                mIterator = mIterator->next;
            }
            else
            {
                element = NULL;
                mIterator = NULL;
            }
        }

        return element;
    }

    template <typename Element>
    Element LinkedList<Element>::getCurrent()
    {
        return mIterator->data;
    }

    template <typename Element>
    Element LinkedList<Element>::getNext(const Element element)
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
    uint32 LinkedList<Element>::getCapacity()
    {
        return mPool->getCapacity();
    }

    template <typename Element>
    uint32 LinkedList<Element>::getSize()
    {
        return mSize;
    }

    template <typename Element>
    uint32 LinkedList<Element>::getSizeOfNode()
    {
        return sizeof(Node);
    }

} // namespace Berserk

#endif //BERSERKENGINE_LINKEDLIST_H
