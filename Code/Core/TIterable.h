/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TITERABLE_H
#define BERSERK_TITERABLE_H

#include <Typedefs.h>

namespace Berserk {

    /**
     * Container, which allows to iterate over elements and do some action
     * @tparam T Type of stored elements
     */
    template <typename T>
    class TIterable {
    public:
        virtual ~TIterable() = default;
        virtual void forEach(const Function<void(T&)> &function) = 0;
        virtual void forEach(const Function<void(const T&)> &function) const = 0;
    };
}

#endif //BERSERK_TITERABLE_H