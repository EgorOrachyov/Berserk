//
// Created by Egor Orachyov on 02.06.2019.
//

#ifndef BERSERK_TLIST_H
#define BERSERK_TLIST_H

#include <Misc/Types.h>
#include <Containers/TIterator.h>
#include <Memory/IAllocator.h>

namespace Berserk
{

    /**
     * Sequent flat hierarchy list container interface with default
     * access functions: add, iterate, remove, get, sort. Base class for
     * array and linked list implementations.
     *
     * @tparam T Template type for elements of the list
     */
    template <typename T>
    class TList : public TIterator<T>
    {
    public:

        /**
         * Adds element in the end of the container.
         * If the container is full, will expand buffer
         * @param element Const reference to the data, which content will
         *                copied by memcpy default implementation
         */
        virtual void add(const T& element) = 0;

        /**
         * Adds uninitialized element (without default constructor call)
         * in the end of the container
         * @return Pointer to the memory of this uninitialized element
         */
        virtual T* addUninitialized() = 0;

        /**
         * Adds specified number of elements in the end of the array without
         * initialization (i.e. without calling default constructor)
         * @param count Number of elements to add
         * @return Size of the container before the addition
         */
        virtual uint32 addUninitialized(uint32 count) = 0;

        /**
         * Adds elements of the IList container in the end of the
         * @param container IList with element to append
         */
        virtual void append(const TList<T> &container) = 0;

        /**
         * Get element via index
         * @warning Assert fail on index out of bounds
         * @param index Of the element to get
         * @return Element reference
         */
        virtual T &get(uint32 index) const = 0;

        /**
         * Remove element via index
         * @warning Assert fail on index out of bounds
         * @param index Of the element to remove
         */
        virtual void remove(uint32 index) = 0;

        /**
         * Removes all the elements in the container
         */
        virtual void clear() = 0;

        /**
         * Allows to sort elements in the order (<), which satisfies the
         * compare predicate function. So if predicate(a,b) => a < b.
         *
         * Requires elements to be movable. Moves elements in the memory.
         * Invalidates indices of the data.
         *
         * @param predicate function (a,b): bool, which satisfies (<) order
         */
        virtual void sort(bool (*predicate)(const T& a, const T& b)) = 0;

        /**
         * @return Current number of elements in the container
         */
        virtual uint32 getSize() const = 0;

        /**
         * Capacity for pre-allocatable containers
         * Or capacity of internal memory allocator
         *
         * @return Max number of the elements, which could be stored
         *         without memory allocation expanding
         */
        virtual uint32 getCapacity() const = 0;

        /**
         * @return Memory usage in bytes by this container
         */
        virtual uint32 getMemoryUsage() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_TLIST_H