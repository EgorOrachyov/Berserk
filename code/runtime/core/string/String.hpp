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

#ifndef BERSERK_STRING_HPP
#define BERSERK_STRING_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>

#include <string>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class String
 * @brief Utf-8 encoded std based default string class
 */
using String = std::string;
using String8u = String;

/**
 * @class StringUtils
 * @brief Utils to work with default string class
 */
class BRK_API StringUtils {
public:
    using CharType = char;

    /** @return Convert string content to value */
    static float ToFloat(const String &str);

    /** @return Convert string content to value */
    static double ToDouble(const String &str);

    /** @return Convert string content to value */
    static int32 ToInt32(const String &str);

    /** @return Convert string content to value */
    static int64 ToInt64(const String &str);

    /** @return Convert string content to value */
    static uint32 ToUint32(const String &str);

    /** @return Convert string content to value */
    static uint64 ToUint64(const String &str);

    /** @return Converted value to string */
    static String From(int value);

    /** @return Converted value to string */
    static String From(unsigned int value);

    /** @return Converted value to string */
    static String From(long value);

    /** @return Converted value to string */
    static String From(unsigned long value);

    /** @return Converted value to string */
    static String Fromf(float value, uint32 precision = 5);

    /** @return Converted value to string */
    static String Fromd(double value, uint32 precision = 5);

    /** @return Converted value to string */
    static String Fromi32(int32 value);

    /** @return Converted value to string */
    static String Fromi64(int64 value);

    /** @return Converted value to string */
    static String Fromu32(uint32 value);

    /** @return Converted value to string */
    static String Fromu64(uint64 value);

    /** @return Converted value to string */
    static String Fromb(bool value);

    /** @return Converted value to string */
    static String Fromp(const void *value);
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_STRING_HPP
