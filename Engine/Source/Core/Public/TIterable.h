//
// Created by Egor Orachyov on 2019-10-13.
//

#ifndef BERSERK_TITERABLE_H
#define BERSERK_TITERABLE_H

#include <TPredicate.h>

namespace Berserk {

    /**
     * Allows to iterate over elements of some container
     * @tparam T type of the element for iteration
     */
    template <typename T>
    class TIterable {
    public:
        /**
         * Called for each element of the container
         * @param function Consumer function to do some action with element
         */
        virtual void forEach(const typename TPredicate::Consume<T>::type &function) = 0;
    };

}

#endif //BERSERK_TITERABLE_H