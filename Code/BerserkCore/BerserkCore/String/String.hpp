/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_STRING_HPP
#define BERSERK_STRING_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/String/StringUtils.hpp>

namespace Berserk {
    namespace String {

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
            using CharType = char;
            /** String utility for char unit */
            using Utils = TStringUtils<CharType, END>;

            String() = default;
            String(const CharType* str);
            String(uint32 capacity);
            String(const CharType* str, uint32 length);
            String(const String& str);
            String(String&& str) noexcept;

            ~String();

            String& operator=(const String& other);
            String& operator=(String&& other) noexcept;

            /** @return Concatenated this with other string */
            String& Append(const String& other);

            /** @return Concatenated this with other string */
            String& operator+=(const String& other);

            /** @return Concatenated this and other string */
            String operator+(const String& other) const;

            /** @return lexicographical '==' */
            bool operator==(const String& other) const;

            /** @return lexicographical '!=' */
            bool operator!=(const String& other) const;

            /** @return lexicographical '<=' */
            bool operator<=(const String& other) const;

            /** @return lexicographical '>=' */
            bool operator>=(const String& other) const;

            /** @return lexicographical '<' */
            bool operator<(const String& other) const;

            /** @return lexicographical '>' */
            bool operator>(const String& other) const;

            /** @return String max CharType units capacity (including null-terminator) */
            uint32 GetCapacity() const { return IsStatic()? CONST_BUFFER_SIZE: mCapacity; }

            /** @return String length in CharType units */
            uint32 GetLength() const { return Utils::Length(GetStr()); }

            /** @return C compatible null-terminated string buffer */
            const CharType* GetStr() const { return IsStatic()? mStatic: mDynamic; }

            /** @return C compatible null-terminated string buffer */
            CharType* GetStr() { return IsStatic()? mStatic: mDynamic; }

            //uint32 Hash() const { return Crc32::Hash(GetStr(), GetLength()); }

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
            static String From(float value, uint32 precision = 5);

            /** @return Converted value to string */
            static String From(double value, uint32 precision = 5);

            /** @return Converted value to string */
            static String From(int32 value);

            /** @return Converted value to string */
            static String From(int64 value);

            /** @return Converted value to string */
            static String From(uint32 value);

            /** @return Converted value to string */
            static String From(uint64 value);

        private:
            bool IsStatic() const { return mCapacity == 0; }
            bool IsDynamic() const { return mCapacity != 0; }

            void* AllocateBuffer(uint32 capacity);
            void DeallocateBuffer(CharType* memory);
            void AlignCapacity(uint32& capacity);

            void StoreString(const CharType* str, uint32 length);
            void AppendAndStoreString(const CharType* str, uint32 length);
            void ConcatenateAndStoreStrings(const CharType* str1, uint32 len1, const CharType* str2, uint32 len2);

        private:
            uint32 mCapacity = 0;

            union {
                CharType* mDynamic;
                CharType mStatic[CONST_BUFFER_SIZE] = { END };
            };
        };

    }
}

#endif //BERSERK_STRING_HPP