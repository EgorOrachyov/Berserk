/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TYPEDEFS_HPP
#define BERSERK_TYPEDEFS_HPP

#include <cinttypes>
#include <memory>

namespace Berserk {

    // Basic types defs

    using int8 = int8_t;
    using uint8 = uint8_t;

    using int16 = int16_t;
    using uint16 = uint16_t;

    using int32 = int32_t;
    using uint32 = uint32_t;

    using int64 = int64_t;
    using uint64 = uint64_t;

    using size_t = std::size_t;
    using wchar = wchar_t;

    // Assumptions about types sizes

    static_assert(sizeof(int8) == 1, "Unexpected type size");
    static_assert(sizeof(uint8) == 1, "Unexpected type size");

    static_assert(sizeof(int16) == 2, "Unexpected type size");
    static_assert(sizeof(uint16) == 2, "Unexpected type size");

    static_assert(sizeof(int32) == 4, "Unexpected type size");
    static_assert(sizeof(uint32) == 4, "Unexpected type size");

    static_assert(sizeof(int64) == 8, "Unexpected type size");
    static_assert(sizeof(uint64) == 8, "Unexpected type size");

    // Macro to work with text and string literals

    // General macro for literals wrapping
    #define BERSERK_TEXT(text) u8 ## text

    // Utf-8 macro for strings
    #define BERSERK_TEXT_UTF8(text) u8 text

}

#endif //BERSERK_TYPEDEFS_HPP
