/**********************************************************************************/
/* This file is part of:                                                          */
/* Berserk Engine project                                                         */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018-2019 Egor Orachyov                                          */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_TLIST_H
#define BERSERK_TLIST_H

#include <HAL/Platform.h>
#include <TIterable.h>
#include <initializer_list>
#include <forward_list>
#include <TVariant.h>

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
    class TList : public TIterable<T>
    {
    public:

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
         * Adds count elements in the end of the container from raw
         * buffer, referenced by pointer array
         * @param array Pointer to buffer with raw data
         * @param count Number of elements in buffer to append
         */
        virtual void append(const T* array, uint32 count) = 0;

        /**
         * Adds elements in the end of the container from initializer list
         * @param list Initializer list with elements
         */
        virtual void append(const std::initializer_list<T>& list) = 0;

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
         * @return Variant T object
         */
        virtual TVariant<T*> find(const typename TPredicate::Satisfy<T>::type &predicate) const = 0;

        /**
         * Remove element via index
         * @warning Assert fail on index out of bounds
         * @param index Of the element to remove
         */
        virtual void remove(uint32 index) = 0;

        /** Removes all the elements in the container */
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
        virtual void sort(const typename TPredicate::Compare<T>::type &predicate) = 0;

        /** @return Current number of elements in the container */
        virtual uint32 getSize() const = 0;

        /** @return Dynamic memory usage in bytes by this container */
        virtual uint32 getMemoryUsage() const = 0;

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
        T& emplace(TArgs&& ... args) {
            T* memory = addUninitialized();
            T* t = new(memory) T(std::forward<TArgs>(args) ...);
            return *t;
        }

    };

} // namespace Berserk

#endif //BERSERK_TLIST_H