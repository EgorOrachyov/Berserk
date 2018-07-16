//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_SHAREDLIST_H
#define BERSERKENGINE_SHAREDLIST_H

#include "Essential/Types.h"
#include "Essential/Assert.h"
#include "Essential/UsageDescriptors.h"

#include "Memory/PoolAllocator.h"

namespace Berserk
{

    /**
     * @addtogroup Containers
     *
     * @{
     */

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

    /**
     * @}
     */

} // namespace Berserk

#include "Source/SharedList.cpp"

#endif //BERSERKENGINE_SHAREDLIST_H
