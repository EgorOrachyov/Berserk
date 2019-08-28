//
// Created by Egor Orachyov on 02.06.2019.
//

#ifndef BERSERK_TLIST_H
#define BERSERK_TLIST_H

#include <Misc/Types.h>
#include <Containers/TIterator.h>

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
    class CORE_EXPORT TList : public TIterator<T>
    {
    public:

        /** Compare predicate type */
        typedef bool (*Predicate)(const T& a, const T& b);

        /** Find predicate type */
        typedef bool (*Satisfy)(const T& a);

    public:

        /**
         * Declare virtual destructor for containers hierarchy
         */
        virtual ~TList() = default;

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
         * Allows to create complex object, which does not support movement
         * semantic in the memory or has complex structure
         * (for example: containers, strings, resources...)
         *
         * Adds created object in the end of the container
         *
         * @warning T type of object must support new/delete semantic of the engine
         *
         * @tparam TArgs Type of arguments, used to create new instance of object T
         * @param args Actual arguments, which will be used to create new instance
         */
        template <typename ... TArgs>
        T& emplace(TArgs&& ... args)
        {
            T* memory = addUninitialized();
            T* t = new(memory) T(std::forward<TArgs>(args) ...);
            return *t;
        }

        /**
         * Allows to create complex object, which does not support movement
         * semantic in the memory or has complex structure
         * (for example: containers, strings, resources...)
         *
         * Adds created object in the end of the container
         *
         * @warning T type of object must support new/delete semantic of the engine
         */
        T& emplace_no_args()
        {
            T* memory = addUninitialized();
            T* t = new(memory) T();
            return *t;
        }

        /**
         * Adds elements of the IList container in the end of the
         * @param container IList with element to append
         */
        virtual void append(const TList<T> &container) = 0;

        /**
         * Adds count elements in the end of the container from raw
         * buffer, referenced by pointer array
         * @param array Pointer to buffer with raw data
         * @param count Number of elements in buffer to append
         */
        virtual void append(const T* array, uint32 count) = 0;

        /**
         * Get element via index
         * @warning Assert fail on index out of bounds
         * @param index Of the element to get
         * @return Element reference
         */
        virtual T &get(uint32 index) const = 0;

        /**
         * Finds first element by predicate
         * @param predicate To find first
         * @return Pointer to the element or null if not found
         */
        virtual T* find(Satisfy predicate) const = 0;

        /**
         * Remove element via index
         * @warning Assert fail on index out of bounds
         * @param index Of the element to remove
         */
        virtual void remove(uint32 index) = 0;

        /**
         * Remove first equal element
         * @note Removes first element from container via predicate match
         * @param element To remove
         * @param predicate Compare predicate
         */
        virtual void remove(const T &element, Predicate predicate) = 0;

        /**
         * Removes all the elements in the container
         */
        virtual void clear() = 0;

        /**
         * Removes all the elements in the container
         * Without calling destructors for stored elements
         */
        virtual void clearNoDestructorCall() = 0;

        /**
         * Allows to sort elements in the order (<), which satisfies the
         * compare predicate function. So if predicate(a,b) => a < b.
         *
         * Requires elements to be movable. Moves elements in the memory.
         * Invalidates indices of the data.
         *
         * @param predicate function (a,b): bool, which satisfies (<) order
         */
        virtual void sort(Predicate predicate) = 0;

        /**
         * @return Current number of elements in the container
         */
        virtual uint32 getSize() const = 0;

        /**
         * @return Memory usage in bytes by this container
         */
        virtual uint32 getMemoryUsage() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_TLIST_H