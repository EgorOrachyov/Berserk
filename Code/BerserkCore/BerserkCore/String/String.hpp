/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_STRING_HPP
#define BERSERK_STRING_HPP

#include <BerserkCore/String/TStringUtils.hpp>
#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/Crc32.hpp>

namespace Berserk {

    /**
     * @brief String class
     *
     * Represents dynamic string with char based storage with small-string buffer optimization.
     * This kind of strings are primary used for char data manipulations, ascii encoding,
     * basic num/float type conversions, concatenations, extensions extraction and etc.
     */
    class String {
    public:
        static const uint32 CONST_BUFFER_SIZE = 28;
        static const char END = '\0';
        using CHAR = char;
        using Utils = TStringUtils<CHAR,END>;

        String() = default;
        String(const CHAR* str);
        String(uint32 capacity);
        String(const CHAR* str, uint32 length);
        String(const String& str);
        String(String&& str) noexcept;

        ~String();

        String& operator=(const String& other);
        String& operator=(String&& other) noexcept;

        String& Add(const String& other);

        String& operator+=(const String& other);
        String operator+(const String& other) const;

        bool operator==(const String& other) const;
        bool operator!=(const String& other) const;
        bool operator<=(const String& other) const;
        bool operator>=(const String& other) const;
        bool operator<(const String& other) const;
        bool operator>(const String& other) const;

        uint32 GetCapacity() const { return IsStatic()? CONST_BUFFER_SIZE: mCapacity; }
        uint32 GetLength() const { return Utils::Length(GetStr()); }
        uint32 Hash() const { return Crc32::Hash(GetStr(), GetLength()); }

        const CHAR* GetStr() const { return IsStatic()? mStatic: mDynamic; }
        CHAR* GetStr() { return IsStatic()? mStatic: mDynamic; }

        float ToFloat() const;
        double ToDouble() const;
        int32 ToInt32() const;
        int64 ToInt64() const;
        int32 ToUint32() const;
        int64 ToUint64() const;

        static String ToString(float value, uint32 precision = 5);
        static String ToString(double value, uint32 precision = 5);
        static String ToString(int32 value);
        static String ToString(int64 value);
        static String ToString(uint32 value);
        static String ToString(uint64 value);

    private:
        bool IsStatic() const { return mCapacity == 0; }
        bool IsDynamic() const { return mCapacity != 0; }

        void StoreString(const CHAR* str, uint32 length);
        void AddAndStoreString(const CHAR* str, uint32 length);
        void AddAndStoreStrings(const CHAR* str1, uint32 len1, const CHAR* str2, uint32 len2);

        uint32 mCapacity = 0;

        union {
            CHAR* mDynamic;
            CHAR mStatic[CONST_BUFFER_SIZE] = { END };
        };
    };
    
}

#endif //BERSERK_STRING_HPP