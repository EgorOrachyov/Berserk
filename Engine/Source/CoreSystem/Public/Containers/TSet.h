//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_TSET_H
#define BERSERK_TSET_H

#include <Containers/TIterator.h>

namespace Berserk
{

    /**
     * Generic set interface, which provides add, remove, contains operations.
     * Supports emplacing of complex objects. Also allows to find and iterate through elements.
     *
     * @tparam T Template type for elements of the set
     */
    template <typename T>
    class CORE_EXPORT TSet : public TIterator<T>
    {
    public:

        /**
         * Declare virtual destructor for containers hierarchy
         */
        virtual ~TSet() = default;

        /**
         * Adds element in the end of the container.
         *
         * @note If container already has the element, then this function does not do anything.
         * @note If the container is full, will expand buffer
         *
         * @param element Const reference to the data, which content will
         *                copied by memcpy default implementation
         */
        virtual void add(const T& element) = 0;

        /**
         * Adds elements of the TSet container to this one. Acts as
         * operation of union for sets.
         *
         * @param set Set with elements to append
         */
        virtual void append(const TSet<T> &set) = 0;

        /**
         * Adds elements of the raw array buffer to this set. Acts as
         * operation of union for sets.
         *
         * @param array Pointer to buffer with raw data
         * @param count Number of elements in buffer to append
         */
        virtual void append(const T* array, uint32 count) = 0;

        /**
         * Checks whether this container contains the specified element.
         * @param element Element to check
         * @return True if contains this element
         */
        virtual bool contains(const T& element) const  = 0;

        /**
         * Remove the specified element from the container
         */
        virtual void remove(const T& element) = 0;

        /**
         * Find specified element in the set.
         * @param element To find
         * @return Pointer to the element if it is found, otherwise nullptr
         */
        virtual T* find(const T& element) const = 0;

        /**
         * @return Current number of elements in the container
         */
        virtual uint32 getSize() const = 0;

        /**
         * @return Memory usage in bytes by this container
         */
        virtual uint32 getMemoryUsage() const = 0;

    protected:

        /**
         * Pre-allocates memory for the element
         * @note If the container is full, will expand buffer
         * @return Pointer to the memory of uninitialized element
         */
        virtual T* preallocate() = 0;

    };

} // namespace Berserk

#endif //BERSERK_TSET_H