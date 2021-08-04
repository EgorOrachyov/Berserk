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

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Templates/Contracts.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Strings/BasicString.hpp>

namespace Berserk {

    /**
     * @brief String class
     *
     * Represents dynamic utf-8 encoded string with char based storage
     * with small-string buffer optimization.
     *
     * This kind of strings are primary used for char data manipulations, ascii encoding,
     * basic num/float type conversions, concatenations, extensions extraction and etc.
     *
     * This string uses platform specific string allocator.
     */
    class String: public BasicString<char, '\0', 28>{
    public:
        /** Small buffer capacity */
        using Base = BasicString<char, '\0', 28>;
        /** Null terminator */
        using Base::END;
        /** Char unit */
        using Base::CharType;
        /** String utility for char unit */
        using Base::Utils;
        /** Search query result for string */
        using Base::Result;

        String() = default;
        String(const CharType *str);
        String(uint32 capacity);
        String(const CharType *str, uint32 length);
        String(const String &str);
        String(String &&str) noexcept;

        ~String();

        String &operator=(const String &other);
        String &operator=(String &&other) noexcept;

        /** @return Concatenated this with other string */
        String &Add(const String &other);

        /** @return Concatenated this with char */
        String &Add(CharType character);

        /** @return Concatenated this with other string */
        String &operator+=(const String &other);

        /** @return Concatenated this and other string */
        String operator+(const String &other) const;

        /** @return lexicographical '==' */
        bool operator==(const String &other) const;

        /** @return lexicographical '!=' */
        bool operator!=(const String &other) const;

        /** @return lexicographical '<=' */
        bool operator<=(const String &other) const;

        /** @return lexicographical '>=' */
        bool operator>=(const String &other) const;

        /** @return lexicographical '<' */
        bool operator<(const String &other) const;

        /** @return lexicographical '>' */
        bool operator>(const String &other) const;

        void Split(const String& separator, Array<String> &results) const;

        String SubString(uint32 from, uint32 length) const;

        /** @return String converted to lower case (string mush have correct utf-8 data) */
        String ToLower() const;

        /** @return String converted to lower case (string mush have correct utf-8 data) */
        String ToUpper() const;

        /**
         * Converts this utf-8 encoded string to utf-16 encoded string.
         * @return True if successfully converted
         */
        bool ToUtf16(class String16u& out) const;

        /** @return Convert string content to value */
        float ToFloat() const;

        /** @return Convert string content to value */
        double ToDouble() const;

        /** @return Convert string content to value */
        int32 ToInt32() const;

        /** @return Convert string content to value */
        int64 ToInt64() const;

        /** @return Convert string content to value */
        uint32 ToUint32() const;

        /** @return Convert string content to value */
        uint64 ToUint64() const;

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

        /** @return Concatenated c-style string and other string */
        friend String operator+(const CharType* left, const String& right) { return String(left) + right; }
    };

    /**
     * Implement contracts for the string
     */
    template<>
    class Equals<String> {
    public:
        bool operator()(const String &a, const String &b) const {
            return a == b;
        }
    };

    template<>
    class Hash<String> {
    public:
        uint32 operator()(const String &a) const {
            return a.Hash();
        }
    };

    template<>
    class CompareLess<String> {
    public:
        bool operator()(const String &a, const String &b) const {
            return a < b;
        }
    };

    template <>
    class TextPrint<String> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const String& string) const {
            stream.Add(string);
        }
    };

}

#endif //BERSERK_STRING_HPP