//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_LINKEDLIST_H
#define BERSERKENGINE_LINKEDLIST_H

#include "../Essential/Types.h"
#include "../Essential/Assert.h"
#include "../Essential/UsageDescriptors.h"
#include "../Memory/StaticPoolAllocator.h"

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
        void Init(StaticPoolAllocator* poolAllocator);

        /**
         * Add element to the list
         *
         * @param element Element which should be STORED into list
         */
        void Add(Element element);

        /**
         * Add element to the list
         *
         * @param element Element which should be STORED into list
         */
        void Add(Element* element);

        /**
         * Add element in the front of the list
         *
         * @param element Element which should be STORED into list
         */
        void AddFront(Element element);

        /**
         * Add element in the back of the list
         *
         * @param element Element which should be STORED into list
         */
        void AddBack(Element element);

        /**
         * Removes element from the list if it existsx
         *
         * @param element Element to be removed
         */
        void Remove(Element element);

        /**
         * Empty list and remove all the elements (will be called standard destructors
         * for each element)
         */
        void Empty();

        /**
         * Get first element from the list
         *
         * @return First element
         */
        Element GetFirst();

        /**
         * Get current element of iterator
         *
         * @return Current element
         */
        Element GetCurrent();

        /**
         * Get last element of the list
         *
         * @return Last element
         */
        Element GetLast();

        /**
         * Returns the next element which is stored after param
         *
         * @param element Element to find the next after that
         * @return Element which stays after param element
         */
        Element GetNext(Element element);

        /**
         * Iterator - return null if restart == false or returns
         * current element and moves iterator pointer to the next element
         *
         * @param restart Flag should the iterator begin from start
         * @return
         */
        Element Iterate(bool restart = false);

        /**
         * Get max num of elements which could be stored in the list
         *
         * @return Max num of element
         */
        uint32 GetCapacity();

        /**
         * Get num of elements currently stored in the list
         *
         * @return Current number of elements
         */
        uint32 GetSize();

        /**
         * Get total size of one list Node
         *
         * @return Size of Node
         */
        uint32 GetSizeOfNode();

    private:

        uint32 mSize;
        Node* mHead;
        Node* mTail;
        Node* mIterator;
        StaticPoolAllocator* mPool;

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
        Empty();
    }

    template <typename Element>
    void LinkedList<Element>::Init(StaticPoolAllocator* poolAllocator)
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
    void LinkedList<Element>::Add(const Element element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*)mPool->AllocBlock();
            mHead->data = element;
            mHead->next = NULL;

            mTail = mHead;
        }
        else
        {
            mTail->next = (Node*)mPool->AllocBlock();
            mTail = mTail->next;
            mTail->data = element;
            mTail->next = NULL;
        }

        mSize += 1;
    }

    template <typename Element>
    void LinkedList<Element>::Add(Element* const element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*)mPool->AllocBlock();
            memcpy(&mHead->data, element, sizeof(Element));
            mHead->next = NULL;

            mTail = mHead;
        }
        else
        {
            mTail->next = (Node*)mPool->AllocBlock();
            mTail = mTail->next;
            memcpy(&mTail->data, element, sizeof(Element));
            mTail->next = NULL;
        }

        mSize += 1;
    }

    template <typename Element>
    void LinkedList<Element>::AddFront(const Element element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*)mPool->AllocBlock();
            mHead->data = element;
            mHead->next = NULL;

            mTail = mHead;
        }
        else
        {
            Node* newHead = (Node*)mPool->AllocBlock();
            newHead->next = mHead;
            newHead->data = element;
            mHead = newHead;
        }

        mSize += 1;
    }

    template <typename Element>
    void LinkedList<Element>::AddBack(const Element element)
    {
        if (mHead == NULL)
        {
            mHead = (Node*)mPool->AllocBlock();
            mHead->data = element;
            mHead->next = NULL;

            mTail = mHead;
        }
        else
        {
            mTail->next = (Node*)mPool->AllocBlock();
            mTail = mTail->next;
            mTail->data = element;
            mTail->next = NULL;
        }

        mSize += 1;
    }

    template <typename Element>
    void LinkedList<Element>::Remove(const Element element)
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
                    mPool->FreeBlock((void*)tmp);
                }
                else
                {
                    mHead = tmp->next;
                    mPool->FreeBlock((void*)tmp);
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
    void LinkedList<Element>::Empty()
    {
        Node* next;
        Node* tmp = mHead;
        while (tmp)
        {
            next = tmp->next;
            tmp->data.~Element();
            mPool->FreeBlock((void*)tmp);
            tmp = next;
        }

        mSize = 0;
        mHead = NULL;
        mTail = NULL;
        mIterator = NULL;
    }

    template <typename Element>
    Element LinkedList<Element>::GetFirst()
    {
        ASSERT(mHead, "Data should be allocated before usage");
        return mHead->data;
    }

    template <typename Element>
    Element LinkedList<Element>::GetLast()
    {
        ASSERT(mTail, "Data should be allocated before usage");
        return mTail->data;
    }

    template <typename Element>
    Element LinkedList<Element>::Iterate(bool restart)
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
    Element LinkedList<Element>::GetCurrent()
    {
        return mIterator->data;
    }

    template <typename Element>
    Element LinkedList<Element>::GetNext(const Element element)
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
    uint32 LinkedList<Element>::GetCapacity()
    {
        return mPool->GetCapacity();
    }

    template <typename Element>
    uint32 LinkedList<Element>::GetSize()
    {
        return mSize;
    }

    template <typename Element>
    uint32 LinkedList<Element>::GetSizeOfNode()
    {
        return sizeof(Node);
    }

} // namespace Berserk

#endif //BERSERKENGINE_LINKEDLIST_H
