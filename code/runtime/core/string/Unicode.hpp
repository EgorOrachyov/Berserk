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

#ifndef BERSERK_UNICODE_HPP
#define BERSERK_UNICODE_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/string/String.hpp>
#include <core/string/String16u.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class Unicode
 * @brief Unicode encoding utils
 */
class BRK_API Unicode {
public:
    using Char32u = char32_t;
    using Char16u = char16_t;
    using Char8u = char;

    /** Convert code point to lower case */
    static Char32u ToLower(Char32u ch);

    /** Convert code point to lower case */
    static Char32u ToUpper(Char32u ch);

    /** Convert utf-32 point to utf-8 */
    static bool Utf32toUtf8(Char32u ch, Char8u *out, uint32 &len);

    /** Convert utf-8 point to utf-32 */
    static bool Utf8toUtf32(const Char8u *in, uint32 &len, Char32u &out);

    /** Convert utf-32 point to utf-16 point */
    static bool Utf32ToUtf16(Char32u ch, Char16u *out, uint32 &outLen);

    /** Convert utf-16 point to utf-32 point */
    static bool Utf16ToUtf32(const Char16u *in, uint32 &len, Char32u &out);

    /**
     * Convert utf-8 encoded string to utf-16 encoded string
     *
     * @param in String to convert
     * @param[out] out String to store result
     *
     * @return True if successfully converted
     */
    static bool ConvertUtf8ToUtf16(const String8u &in, String16u &out);

    /**
    * Convert utf-16 encoded string to utf-8 encoded string
    *
    * @param in String to convert
    * @param[out] out String to store result
    *
    * @return True if successfully converted
    */
    static bool ConvertUtf16ToUtf8(const String16u &in, String8u &out);
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_UNICODE_HPP
