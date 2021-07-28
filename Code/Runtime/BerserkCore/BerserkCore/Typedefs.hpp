/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_TYPEDEFS_HPP
#define BERSERK_TYPEDEFS_HPP

#include <cinttypes>
#include <memory>
#include <functional>

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

    using wchar = wchar_t;

    template <typename T>
    using Function = std::function<T>;

    // Assumptions about types sizes

    static_assert(sizeof(char) == 1, "Unexpected type size");
    static_assert(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4, "Unexpected type size");

    static_assert(sizeof(int8) == 1, "Unexpected type size");
    static_assert(sizeof(uint8) == 1, "Unexpected type size");

    static_assert(sizeof(int16) == 2, "Unexpected type size");
    static_assert(sizeof(uint16) == 2, "Unexpected type size");

    static_assert(sizeof(int32) == 4, "Unexpected type size");
    static_assert(sizeof(uint32) == 4, "Unexpected type size");

    static_assert(sizeof(int64) == 8, "Unexpected type size");
    static_assert(sizeof(uint64) == 8, "Unexpected type size");
}

#endif //BERSERK_TYPEDEFS_HPP
