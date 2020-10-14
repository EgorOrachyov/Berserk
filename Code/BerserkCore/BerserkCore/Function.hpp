/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_FUNCTION_HPP
#define BERSERK_FUNCTION_HPP

#include <functional>

namespace Berserk {

    template <typename T>
    using Function = std::function<T>;

}

#endif //BERSERK_FUNCTION_HPP