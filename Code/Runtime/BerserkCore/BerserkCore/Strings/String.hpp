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
#include <BerserkCore/Strings/StringUtils.hpp>
#include <BerserkCore/Templates/Array.hpp>

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
    class String {
    public:
        /** Small buffer capacity */
        static const uint32 CONST_BUFFER_SIZE = 28;
        /** Null terminator */
        static const char END = '\0';
        /** Char unit */
        using Char8u = char;
        /** String utility for char unit */
        using Utils = TStringUtils<Char8u, END>;

        /** Search query result for string */
        struct Result {
            enum class Status {
                Found,
                NotFound
            };

            Result() = default;
            explicit Result(uint32 index) : result(Status::Found), index(index) {}

            explicit operator bool() const { return result == Status::Found; }

            const Status result = Status::NotFound;
            const uint32 index = 0;
        };

        String() = default;
        String(const Char8u *str);
        String(uint32 capacity);
        String(const Char8u *str, uint32 length);
        String(const String &str);
        String(String &&str) noexcept;

        ~String();

        String &operator=(const String &other);
        String &operator=(String &&other) noexcept;

        /** @return Concatenated this with other string */
        String &Add(const String &other);

        /** @return Concatenated this with char */
        String &Add(Char8u character);

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

        Result FindFirst(const char* substring) const;

        Result FindLast(const char* substring) const;

        /** @return String converted to lower case (string mush have correct utf-8 data) */
        String ToLower() const;

        /** @return String converted to lower case (string mush have correct utf-8 data) */
        String ToUpper() const;

        /** @return String max CharType units capacity (including null-terminator) */
        uint32 GetCapacity() const { return IsStatic() ? CONST_BUFFER_SIZE : mCapacity; }

        /** @return String length in CharType units */
        uint32 GetLength() const { return Utils::Length(GetStr_C()); }

        /** @return C compatible null-terminated string buffer */
        const Char8u *GetStr_C() const { return IsStatic() ? mStatic : mDynamic; }

        /** @return C compatible null-terminated string buffer */
        Char8u *GetStr_C() { return IsStatic() ? mStatic : mDynamic; }

        /** @return Hash of the string content */
        uint32 Hash() const;

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
        friend String operator+(const Char8u* left, const String& right) { return String(left) + right; }

    private:
        bool IsStatic() const { return mCapacity == 0; }
        bool IsDynamic() const { return mCapacity != 0; }

        uint32 GetOffsetOf(const char* ptr) const { return GetOffsetOf(GetStr_C(), ptr); }
        static uint32 GetOffsetOf(const char* source, const char* ptr) { return (uint32)((uint64)ptr - (uint64)source); }

        void *AllocateBuffer(uint32 capacity);
        void DeallocateBuffer(Char8u *memory, uint64 capacity);

        void AlignCapacity(uint32 &capacity);
        void StoreString(const Char8u *str, uint32 length);

        void AppendAndStoreString(const Char8u *str, uint32 length);
        void ConcatenateAndStoreStrings(const Char8u *str1, uint32 len1, const Char8u *str2, uint32 len2);

    private:
        uint32 mCapacity = 0;

        union {
            Char8u *mDynamic;
            Char8u mStatic[CONST_BUFFER_SIZE] = {END};
        };
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