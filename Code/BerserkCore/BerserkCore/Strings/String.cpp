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
        BERSERK_ASSERT(str);

        if (str) {
            StoreString(str, Utils::Length(str));
        }
    }

    String::String(const String::CharType *str, uint32 length) {
        BERSERK_ASSERT(str);

        if (str) {
            StoreString(str, length);
        }
    }

    String::String(const String &str) {
        StoreString(str.GetStr_C(), str.GetLength());
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
            DeallocateBuffer(mDynamic, GetCapacity());
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

    String &String::Add(const String &other) {
        AppendAndStoreString(other.GetStr_C(), other.GetLength());
        return *this;
    }

    String& String::Add(CharType character) {
        CharType buffer[2] = { character, END };
        AppendAndStoreString(buffer, 1);
        return *this;
    }

    String &String::operator+=(const String &other) {
        AppendAndStoreString(other.GetStr_C(), other.GetLength());
        return *this;
    }

    String String::operator+(const String &other) const {
        String result;
        result.ConcatenateAndStoreStrings(GetStr_C(), GetLength(), other.GetStr_C(), other.GetLength());
        return result;
    }

    bool String::operator==(const String &other) const {
        return Utils::Compare(GetStr_C(), other.GetStr_C()) == 0;
    }

    bool String::operator!=(const String &other) const {
        return Utils::Compare(GetStr_C(), other.GetStr_C()) != 0;
    }

    bool String::operator<=(const String &other) const {
        return Utils::Compare(GetStr_C(), other.GetStr_C()) <= 0;
    }

    bool String::operator>=(const String &other) const {
        return Utils::Compare(GetStr_C(), other.GetStr_C()) >= 0;
    }

    bool String::operator<(const String &other) const {
        return Utils::Compare(GetStr_C(), other.GetStr_C()) < 0;
    }

    bool String::operator>(const String &other) const {
        return Utils::Compare(GetStr_C(), other.GetStr_C()) > 0;
    }

    void String::Split(const String &separator, Array<String> &results) const {
        const CharType* buffer = GetStr_C();
        const CharType* sep = separator.GetStr_C();

        uint32 sepLength = separator.GetLength();
        const CharType* ptr;

        do {
            ptr = Utils::FindFirst(buffer, sep);

            if (ptr) {
                auto offset = GetOffsetOf(buffer, ptr);

                if (offset > 0) {
                    String part(buffer, offset);
                    results.Move(part);
                }

                buffer = &buffer[offset + sepLength];
            }
        } while (ptr != nullptr);

        if (*buffer != END) {
            String part(buffer);
            results.Move(part);
        }
    }

    String String::SubString(uint32 from, uint32 length) const {
        if (from >= GetLength())
            return "";

        // todo
        String result(length + 1);
        Utils::Substring(result.GetStr_C(), GetStr_C(), from, length);
        return result;
    }

    String::Result String::FindFirst(const char *substring) const {
        auto ptr = Utils::FindFirst(GetStr_C(), substring);
        return ptr ? Result(GetOffsetOf(ptr)) : Result();
    }

    String::Result String::FindLast(const char *substring) const {
        auto ptr = Utils::FindLast(GetStr_C(), substring);
        return ptr ? Result(GetOffsetOf(ptr)) : Result();
    }

    uint32 String::Hash() const {
        return Crc32::Hash(GetStr_C(), GetLength());
    }

    float String::ToFloat() const {
        return strtof(GetStr_C(), nullptr);
    }

    double String::ToDouble() const {
        return strtod(GetStr_C(), nullptr);
    }

    int32 String::ToInt32() const {
        const int32 BASE = 10;
        return (int32) strtoll(GetStr_C(), nullptr, BASE);
    }

    int64 String::ToInt64() const {
        const int32 BASE = 10;
        return (int64) strtoll(GetStr_C(), nullptr, BASE);
    }

    uint32 String::ToUint32() const {
        const int32 BASE = 10;
        return (uint32) strtoull(GetStr_C(), nullptr, BASE);
    }

    uint64 String::ToUint64() const {
        const int32 BASE = 10;
        return (uint64) strtoull(GetStr_C(), nullptr, BASE);
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

    String String::From(size_t value) {
        CharType buffer[128];
        snprintf(buffer, 128, "%llu", (unsigned long long int) value);
        return buffer;
    }

    String String::From(bool value) {
        return value? BERSERK_TEXT("True"): BERSERK_TEXT("False");
    }

    String String::From(const void* value) {
        CharType buffer[128];
        snprintf(buffer, 128, "%p", value);
        return buffer;
    }

    void *String::AllocateBuffer(uint32 capacity) {
        return Platform::System::Impl::Instance().AllocateStringBuffer(capacity * sizeof(CharType));
    }

    void String::DeallocateBuffer(CharType *memory, size_t capacity) {
        Platform::System::Impl::Instance().DeallocateStringBuffer(memory, capacity * sizeof(CharType));
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

        GetStr_C()[length] = END;
    }

    void String::AppendAndStoreString(const String::CharType *str, uint32 length) {
        using namespace Platform;

        uint32 myLength = GetLength();
        uint32 newLength = myLength + length;

        if (newLength < GetCapacity()) {
            Memory::Copy(GetStr_C() + myLength, str, length * sizeof(CharType));
        } else {
            uint32 newCapacity = newLength + 1;
            AlignCapacity(newCapacity);
            auto *newDynamic = (CharType *) AllocateBuffer(newCapacity);

            Memory::Copy(newDynamic, GetStr_C(), myLength * sizeof(CharType));
            Memory::Copy(newDynamic + myLength, str, length * sizeof(CharType));

            if (IsDynamic()) {
                DeallocateBuffer(mDynamic, GetCapacity());
            }

            mCapacity = newCapacity;
            mDynamic = newDynamic;
        }

        GetStr_C()[newLength] = END;
    }

    void String::ConcatenateAndStoreStrings(const String::CharType *str1, uint32 len1, const String::CharType *str2,
                                            uint32 len2) {
        using namespace Platform;

        uint32 newLength = len1 + len2;

        if (newLength < GetCapacity()) {
            Memory::Copy(GetStr_C(), str1, len1 * sizeof(CharType));
            Memory::Copy(GetStr_C() + len1, str2, len2 * sizeof(CharType));
        } else {
            uint32 newCapacity = newLength + 1;
            AlignCapacity(newCapacity);
            CharType *newDynamic = (CharType *) AllocateBuffer(newCapacity);

            Memory::Copy(newDynamic, str1, len1 * sizeof(CharType));
            Memory::Copy(newDynamic + len1, str2, len2 * sizeof(CharType));

            if (IsDynamic()) {
                DeallocateBuffer(mDynamic, GetCapacity());
            }

            mCapacity = newCapacity;
            mDynamic = newDynamic;
        }

        GetStr_C()[newLength] = END;
    }

}