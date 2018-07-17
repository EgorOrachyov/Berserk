//
// Created by Egor Orachyov on 06.07.2018.
//

#ifndef BERSERKENGINE_LINKEDLIST_H
#define BERSERKENGINE_LINKEDLIST_H

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
     * Linked list container based up on PoolAllocator. Store data (not pointers),
     * expand internal buffer if there is not enough memory
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
        void remove(Element& element);

        /**
         * Empty list and remove all the elements (will be called standard destructors
         * for each element)
         */
        void empty();

        /**
         * Get element with i index
         *
         * @param i Element index in [0;size)
         * @return Element
         */
        Element& get(UINT32 i) const ;

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

#include "Source/LinkedList.cpp"

#endif //BERSERKENGINE_LINKEDLIST_H
