//
// Created by Egor Orachyov on 2019-08-22.
//

#ifndef BERSERK_TREMOVEITERATOR_H
#define BERSERK_TREMOVEITERATOR_H

#include <Containers/TIterator.h>

namespace Berserk
{

    /**
     * Remove iterator allows to modify container in
     * time of the iteration.
     *
     * @tparam T Template type for elements of the iterator
     */
    template <typename T>
    class CORE_EXPORT TRemoveIterator : public TIterator<T>
    {
    public:

        /**
         * Declare virtual destructor for containers hierarchy
         */
        ~TRemoveIterator() override = default;

        /**
         * Removes current element of the iterations and goes
         * to the next element. If next element is not present, then
         * it is null and iterating is stopped.
         */
        virtual void removeCurrent() = 0;

    };

}

#endif //BERSERK_TREMOVEITERATOR_H