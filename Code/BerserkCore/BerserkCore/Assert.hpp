/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_ASSERT_HPP
#define BERSERK_ASSERT_HPP

#include <BerserkCore/Error/Exception.hpp>

#define BERSERK_ASSERT(condition)                       \
    if (!(condition)) {                                 \
        BERSERK_EXCEPT(AssertException, #condition);    \
    }                                                   \
    else { }                                            \

#endif //BERSERK_ASSERT_HPP