/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_DELEGATE_HPP
#define BERSERK_DELEGATE_HPP

#include <functional>

namespace Berserk {

    template <typename T>
    using Delegate = std::function<T>;

}

#endif //BERSERK_DELEGATE_HPP