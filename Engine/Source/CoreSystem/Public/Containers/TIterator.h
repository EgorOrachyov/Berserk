//
// Created by Egor Orachyov on 02.06.2019.
//

#ifndef BERSERK_ITERATOR_H
#define BERSERK_ITERATOR_H

#include <Misc/UsageDescriptors.h>

namespace Berserk
{

    /**
     * Iterator interface for iterable containers types.
     * Allows to go through the storage and access all the elements sequentially.
     *
     * @tparam T Template type for elements of the iterator
     */
    template <typename T>
    class CORE_EXPORT TIterator
    {
    public:

        /**
         * Declare virtual destructor for containers hierarchy
         */
        virtual ~TIterator() = default;

        /**
         * Begins iterating through the container from the first element.
         * @return Null if the container is empty, otherwise pointer to the
         *         first element of the iterating
         */
        virtual T* begin() const = 0;

        /**
         * Access next element in the iterating sequence.
         * @return Null if it is the end of the container, otherwise pointer
         *         to the next element after current
         */
        virtual T* next() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_ITERATOR_H