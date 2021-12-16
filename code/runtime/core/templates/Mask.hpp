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

#ifndef BERSERK_MASK_HPP
#define BERSERK_MASK_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>

#include <bitset>
#include <cassert>
#include <initializer_list>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class Mask
 * @brief Type-safe enum mask wrapper
 *
 * @tparam T Enum type
 * @tparam Size Enum values count
 */
template<typename T, size_t Size = sizeof(uint32) * 8>
class Mask {
public:
    Mask() = default;
    Mask(const std::initializer_list<T> &list) {
        for (auto v : list) {
            Set(v);
        }
    }

    Mask &Set(T v, bool val = true) {
        assert(static_cast<size_t>(v) < Size);
        auto index = static_cast<size_t>(v);
        value.set(index, val);
        return *this;
    }

    bool Get(T v) const {
        assert(static_cast<size_t>(v) < Size);
        auto index = static_cast<size_t>(v);
        return value[index];
    }

    std::bitset<Size> value;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_MASK_HPP
