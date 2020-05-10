/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
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

    using wchar = wchar_t;

    using int8 = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;

    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;

    template <typename T>
    using Function = std::function<T>;

}

#define BERSERK_BIT_SHIFT(N) 1u << N

#endif //BERSERK_TYPEDEFS_H
