//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_SHAREDLIST_H
#define BERSERKENGINE_SHAREDLIST_H

#include "../Essential/Types.h"
#include "../Essential/Assert.h"
#include "../Essential/UsageDescriptors.h"

#include "Memory/PoolAllocator.h"

namespace Berserk
{

    /**
     * Shared list container based up on shared PoolAllocator. Store data (not pointers),
     * expand internal buffer if there is not enough memory
     *
     * @note List does not control pool allocator, therefore you should manually create and serve pool
     */
    template <typename Element> class DATA_API SharedList
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

        SharedList();
        ~SharedList();

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
        void remove(Element& element);

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
        PoolAllocator* mPool;

    };

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
        PUSH("Delete shared list %p shared allocator %p\n", this, mPool);
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

#endif //BERSERKENGINE_SHAREDLIST_H
