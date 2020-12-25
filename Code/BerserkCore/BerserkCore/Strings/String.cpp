/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/Platform/System.hpp>
#include <BerserkCore/Misc/Crc32.hpp>

namespace Berserk {

    String::String(uint32 capacity) {
        if (capacity > CONST_BUFFER_SIZE) {
            mCapacity = capacity;
            AlignCapacity(mCapacity);
            mDynamic = (CharType *) AllocateBuffer(mCapacity);
            mDynamic[0] = END;
        }
    }

    String::String(const CharType *str) {
        StoreString(str, Utils::Length(str));
    }

    String::String(const String::CharType *str, uint32 length) {
        StoreString(str, length);
    }

    String::String(const String &str) {
        StoreString(str.GetStr(), str.GetLength());
    }

    String::String(String &&str) noexcept {
        using namespace Platform;

        if (str.IsStatic()) {
            Memory::Copy(mStatic, str.mStatic, (str.GetLength() + 1) * sizeof(CharType));
            str.mStatic[0] = END;
        } else {
            mCapacity = str.mCapacity;
            mDynamic = str.mDynamic;
            str.mCapacity = 0;
            str.mDynamic = nullptr;
        }
    }

    String::~String() {
        if (IsDynamic()) {
            DeallocateBuffer(mDynamic);
        }

        mDynamic = nullptr;
        mCapacity = 0;
    }

    String &String::operator=(const String &other) {
        if (this == &other) {
            return *this;
        }

        this->~String();
        new(this) String(other);
        return *this;
    }

    String &String::operator=(String &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        this->~String();
        new(this) String(std::move(other));
        return *this;
    }

    String &String::Append(const String &other) {
        AppendAndStoreString(other.GetStr(), other.GetLength());
        return *this;
    }

    String &String::operator+=(const String &other) {
        AppendAndStoreString(other.GetStr(), other.GetLength());
        return *this;
    }

    String String::operator+(const String &other) const {
        String result;
        result.ConcatenateAndStoreStrings(GetStr(), GetLength(), other.GetStr(), other.GetLength());
        return result;
    }

    bool String::operator==(const String &other) const {
        return Utils::Compare(GetStr(), other.GetStr()) == 0;
    }

    bool String::operator!=(const String &other) const {
        return Utils::Compare(GetStr(), other.GetStr()) != 0;
    }

    bool String::operator<=(const String &other) const {
        return Utils::Compare(GetStr(), other.GetStr()) <= 0;
    }

    bool String::operator>=(const String &other) const {
        return Utils::Compare(GetStr(), other.GetStr()) >= 0;
    }

    bool String::operator<(const String &other) const {
        return Utils::Compare(GetStr(), other.GetStr()) < 0;
    }

    bool String::operator>(const String &other) const {
        return Utils::Compare(GetStr(), other.GetStr()) > 0;
    }

    String String::SubString(uint32 from, uint32 length) const {
        if (from >= GetLength())
            return "";

        // todo
        String result(length + 1);
        Utils::Substring(result.GetStr(), GetStr(), from, length);
        return result;
    }

    String::Query String::FindFirst(const char *substring) const {
        auto ptr = Utils::FindFirst(GetStr(), substring);
        return ptr? Query(GetOffsetOf(ptr)) : Query();
    }

    String::Query String::FindLast(const char *substring) const {
        auto ptr = Utils::FindLast(GetStr(), substring);
        return ptr? Query(GetOffsetOf(ptr)) : Query();
    }

    uint32 String::Hash() const {
        return Crc32::Hash(GetStr(), GetLength());
    }

    float String::ToFloat() const {
        return strtof(GetStr(), nullptr);
    }

    double String::ToDouble() const {
        return strtod(GetStr(), nullptr);
    }

    int32 String::ToInt32() const {
        const int32 BASE = 10;
        return (int32) strtoll(GetStr(), nullptr, BASE);
    }

    int64 String::ToInt64() const {
        const int32 BASE = 10;
        return (int64) strtoll(GetStr(), nullptr, BASE);
    }

    uint32 String::ToUint32() const {
        const int32 BASE = 10;
        return (uint32) strtoull(GetStr(), nullptr, BASE);
    }

    uint64 String::ToUint64() const {
        const int32 BASE = 10;
        return (uint64) strtoull(GetStr(), nullptr, BASE);
    }

    String String::From(float value, uint32 precision) {
        CharType buffer[64];
        snprintf(buffer, 64, "%.*f", precision, value);
        return buffer;
    }

    String String::From(double value, uint32 precision) {
        CharType buffer[128];
        snprintf(buffer, 128, "%.*lf", precision, value);
        return buffer;
    }

    String String::From(int32 value) {
        CharType buffer[64];
        snprintf(buffer, 64, "%i", value);
        return buffer;
    }

    String String::From(int64 value) {
        CharType buffer[128];
        snprintf(buffer, 128, "%lli", (long long int) value);
        return buffer;
    }

    String String::From(uint32 value) {
        CharType buffer[64];
        snprintf(buffer, 64, "%u", value);
        return buffer;
    }

    String String::From(uint64 value) {
        CharType buffer[128];
        snprintf(buffer, 128, "%llu", (unsigned long long int) value);
        return buffer;
    }

    String String::From(bool value) {
        return value? BERSERK_TEXT("True"): BERSERK_TEXT("False");
    }

    void *String::AllocateBuffer(uint32 capacity) {
        return Platform::System::Impl::Instance().AllocateStringBuffer(capacity * sizeof(CharType));
    }

    void String::DeallocateBuffer(CharType *memory) {
        Platform::System::Impl::Instance().DeallocateStringBuffer(memory);
    }

    void String::AlignCapacity(uint32 &capacity) {
        // Can assign what ever we want
        // It would be better to set as min the cache line size
        uint32 bytesSize = 64;
        uint32 factor = 2;

        while (bytesSize < capacity * sizeof(CharType)) {
            bytesSize *= factor;
        }

        capacity = bytesSize / sizeof(CharType);
    }

    void String::StoreString(const String::CharType *str, uint32 length) {
        using namespace Platform;

        if (length < CONST_BUFFER_SIZE) {
            Memory::Copy(mStatic, str, length * sizeof(CharType));
        } else {
            mCapacity = length + 1;
            AlignCapacity(mCapacity);
            mDynamic = (CharType *) AllocateBuffer(mCapacity);
            Memory::Copy(mDynamic, str, length * sizeof(CharType));
        }

        GetStr()[length] = END;
    }

    void String::AppendAndStoreString(const String::CharType *str, uint32 length) {
        using namespace Platform;

        uint32 myLength = GetLength();
        uint32 newLength = myLength + length;

        if (newLength < GetCapacity()) {
            Memory::Copy(GetStr() + myLength, str, length * sizeof(CharType));
        } else {
            uint32 newCapacity = newLength + 1;
            AlignCapacity(newCapacity);
            auto *newDynamic = (CharType *) AllocateBuffer(newCapacity);

            Memory::Copy(newDynamic, GetStr(), myLength * sizeof(CharType));
            Memory::Copy(newDynamic + myLength, str, length * sizeof(CharType));

            if (IsDynamic()) {
                DeallocateBuffer(mDynamic);
            }

            mCapacity = newCapacity;
            mDynamic = newDynamic;
        }

        GetStr()[newLength] = END;
    }

    void String::ConcatenateAndStoreStrings(const String::CharType *str1, uint32 len1, const String::CharType *str2,
                                            uint32 len2) {
        using namespace Platform;

        uint32 newLength = len1 + len2;

        if (newLength < GetCapacity()) {
            Memory::Copy(GetStr(), str1, len1 * sizeof(CharType));
            Memory::Copy(GetStr() + len1, str2, len2 * sizeof(CharType));
        } else {
            uint32 newCapacity = newLength + 1;
            AlignCapacity(newCapacity);
            CharType *newDynamic = (CharType *) AllocateBuffer(newCapacity);

            Memory::Copy(newDynamic, str1, len1 * sizeof(CharType));
            Memory::Copy(newDynamic + len1, str2, len2 * sizeof(CharType));

            if (IsDynamic()) {
                DeallocateBuffer(mDynamic);
            }

            mCapacity = newCapacity;
            mDynamic = newDynamic;
        }

        GetStr()[newLength] = END;
    }

}