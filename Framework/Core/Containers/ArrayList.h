//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_ARRAYLIST_H
#define BERSERKENGINE_ARRAYLIST_H

#include "Essential/Types.h"
#include "Essential/Assert.h"
#include "Essential/UsageDescriptors.h"

#include "Logging/LogMessages.h"
#include "Memory/MemoryAllocators.h"

namespace Berserk
{

    /**
     * Dynamic array for elements which allocates memory in the heap if its is not
     * enough for next pushing of element. Should be used for elements with default
     * destructors for simple storing of that in this container.
     *
     * @note If you want to store huge data with complex destructors look at the
     * Linked List container
     */
    template <typename Element> class DATA_API ArrayList
    {
    public:

        /**
         * Initialize empty array list
         */
        ArrayList();

        /**
         * Calls empty
         */
        ~ArrayList();

        /**
         * Initialize list with desired start size
         * @param initialSize (default value 16)
         */
        void init(UINT32 initialSize = 16);

        /**
         * Add new element to the list
         *
         * @note Element will be stored in the list by value, not by reference
         *
         * @param element
         */
        void add(const Element &element);

        /**
         * Remove element with index
         * @param index
         */
        void remove(UINT32 index);

        /**
         * Calls destructors for all the elements, frees buffer's data
         */
        void empty();

        /**
         * Reserve desired number of elements
         *
         * @warning Should be more than 16
         * @warning If current number of elements more than desiredCount they will be destroyed
         * and deleted from the list data
         *
         * @param desiredCount
         */
        void reserve(UINT32 desiredCount);

        /**
         * Number of elements in the list
         * @return Current count
         */
        UINT32 getSize() const;

        /**
         * Max possible umber of elements in the list
         * @return Max count
         */
        UINT32 getCapacity() const;

        /**
         * Size of the element in the bytes
         * @return Size in bytes
         */
        UINT32 getElementSize() const;

        /**
         * Get element with index like a pointer
         *
         * @param index
         * @return Pointer to element
         */
        Element* getPointer(UINT32 index) const;

        /**
         * Get element with index
         *
         * @warning Index should be in [0;size)
         *
         * @param index
         * @return Elemnet
         */
        Element& get(UINT32 index) const;

        /**
         * Get first element
         *
         * @warning List should not be empty
         *
         * @return Element
         */
        Element& getFirst() const;

        /**
         * Get last element
         *
         * @warning List should not be empty
         *
         * @return Element
         */
        Element& getLast() const;

        /**
         * Get pointer to internal elements buffer
         *
         * @return Pointer to data
         */
        Element* getBuffer() const;

    private:

        /**
         * Will expand buffer by 2 if it is need
         */
        void expandBuffer();

    private:

        UINT32 mCapacity;           // Max number of elements in array without expansion
        UINT32 mCurrentSize;        // Current number of element in the array
        Element* mBuffer;           // Pointer to buffer which stores elements in thr heap

    };

} // namespace Berserk

#include "Source/ArrayList.cpp"

#endif //BERSERKENGINE_ARRAYLIST_H