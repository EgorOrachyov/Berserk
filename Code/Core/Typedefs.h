/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TYPEDEFS_H
#define BERSERK_TYPEDEFS_H

#include <stdint.h>
#include <inttypes.h>
#include <assert.h>
#include <functional>

namespace Berserk {

    typedef wchar_t wchar;

    typedef int8_t int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef int64_t int64;

    typedef uint8_t uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;

    typedef float float32;
    typedef double float64;

    template <typename T>
    using Function = std::function<T>;

}

#define BERSERK_BIT_SHIFT(N) 1u << N

#endif //BERSERK_TYPEDEFS_H
