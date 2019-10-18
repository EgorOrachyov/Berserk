//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_TSET_H
#define BERSERK_TSET_H

#include <HAL/Platform.h>
#include <TVariant.h>

namespace Berserk
{

    /**
     * Generic set interface, which provides add, remove, contains operations.
     * Also allows to find and iterate through elements.
     * @tparam T Template type for elements of the set
     */
    template <typename T>
    class TSet
    {
    public:

        virtual ~TSet() = default;

        /**
         * Adds element into set. If set already has the element,
         * then this function does nothing.
         * @param element Element to add
         */
        virtual void add(const T& element) = 0;

        /**
         * Adds elements of the raw array buffer to this set.
         * @param array Pointer to buffer with raw data
         * @param count Number of elements in buffer to append
         */
        virtual void append(const T* array, uint32 count) = 0;

        /**
         * Adds list elements to the set.
         * @param list List with elements to append
         */
        virtual void append(const std::initializer_list<T> &list) = 0;

        /**
         * Checks whether this container contains the specified element.
         * @param element Element to check
         * @return True if contains this element
         */
        virtual bool contains(const T& element) const  = 0;

        /**
         * Find specified element in the set.
         * @param element To find
         * @return Variant element pointer
         */
        virtual TVariant<T*> find(const T& element) const = 0;

        /** Remove the specified element from the container */
        virtual void remove(const T& element) = 0;

        /** @return Current number of elements in the container */
        virtual uint32 getSize() const = 0;

        /** @return Dynamic memory usage in bytes by this container */
        virtual uint32 getMemoryUsage() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_TSET_H