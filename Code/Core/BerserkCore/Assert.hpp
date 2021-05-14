/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_ASSERT_HPP
#define BERSERK_ASSERT_HPP

#include <BerserkCore/Defines.hpp>
#include <cassert>

#ifdef BERSERK_DEBUG
    #define BERSERK_ASSERT(condition) { assert((condition)); }
#else
    #define BERSERK_ASSERT(condition)
#endif // BERSERK_DEBUG

#endif //BERSERK_ASSERT_HPP