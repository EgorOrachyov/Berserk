//
// Created by Egor Orachyov on 27.07.2018.
//

#ifndef BERSERKENGINE_QUEUE_H
#define BERSERKENGINE_QUEUE_H

#include "Essential/Types.h"
#include "Essential/UsageDescriptors.h"

#include "Containers/ArrayList.h"

namespace Berserk
{

    /**
     * @defgroup Containers
     * @brief Data structures
     *
     * Module provides common data structures for engine sub-systems. Note that
     * containers are oriented on STORING data inside that (not handling a pointer)
     *
     * @ingroup Core
     *
     * @{
     */


    /**
     * ArrayList based basic one side list which allows add elements, reserve capacity,
     * lock expanding and clean up for O(1) without calling destructors. (Primary used for render list)
     *
     * @note Allows only to push elements in the list
     * @tparam Element Stored element
     */
    template <typename Element> class DATA_API List
    {
    public:

        /**
         * Init empty list
         */
        List();

        /**
         * Empty() call
         */
        ~List();

        /**
         * Init list with desired initial size
         *
         * @note Use power of two
         * @param initialSize Start size of list (num of elements)
         */
        void init(UINT32 initialSize = 16);

        /**
         * Clean list (size = 0) without calling destructors of elements
         */
        void clean();

        /**
         * Call destructor for each element and FREE INTERNAL buffer
         * @warning After this function call init() before using
         */
        void empty();

        /**
         * Prohibit expanding of internal buffer in add() function if
         * size will be equal to capacity
         */
        void lock();

        /**
         * Permit internal buffer expanding
         */
        void unlock();

        /**
         * Add element in list
         * @param element To add
         */
        void add(const Element &element);

        /**
         * Get element via its index in [0; size)
         * @warning Borders are checked only in debug mode
         * @param index Of element
         * @return Element
         */
        Element& get(UINT32 index) const;

        /**
         * Get the last added element in the list
         * @return Element
         */
        Element& getLast() const;

        /**
         * Number of elements in list
         * @return Size
         */
        UINT32 getSize() const;

        /**
         * Max number of elements in this buffer without expanding of buffer
         * @return Capacity
         */
        UINT32 getCapacity() const;

        /**
         * Is expanding of buffer locked
         * @return True if buffer won't expand in add function
         */
        bool isLocked() const;

    private:

        INT8 mIsLocked : 1;             // Is expanding of ArrayList locked
        ArrayList<Element> mList;       // List elements buffer

    };

    /**
     * @}
     */

} // namespace Berserk

#include "Source/List.cpp"

#endif //BERSERKENGINE_QUEUE_H