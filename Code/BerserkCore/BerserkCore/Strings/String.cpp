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

#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/Platform/System.hpp>
#include <BerserkCore/Platform/Crc32.hpp>
#include <BerserkCore/Strings/Unicode.hpp>

namespace Berserk {

    String::String(uint32 capacity) {
        if (capacity > CONST_BUFFER_SIZE) {
            mCapacity = capacity;
            AlignCapacity(mCapacity);
            mDynamic = (Char8u *) AllocateBuffer(mCapacity);
            mDynamic[0] = END;
        }
    }

    String::String(const Char8u *str) {
        assert(str);

        if (str) {
            StoreString(str, Utils::Length(str));
        }
    }

    String::String(const String::Char8u *str, uint32 length) {
        assert(str);

        if (str) {
            StoreString(str, length);
        }
    }

    String::String(const String &str) {
        StoreString(str.GetStr_C(), str.GetLength());
    }

    String::String(String &&str) noexcept {
        if (str.IsStatic()) {
            Memory::Copy(mStatic, str.mStatic, (str.GetLength() + 1) * sizeof(Char8u));
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

    String& String::Add(Char8u character) {
        Char8u buffer[2] = {character, END };
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
        const Char8u* buffer = GetStr_C();
        const Char8u* sep = separator.GetStr_C();

        uint32 sepLength = separator.GetLength();
        const Char8u* ptr;

        do {
            ptr = Utils::FindFirst(buffer, sep);

            if (ptr) {
                auto offset = GetOffsetOf(buffer, ptr);

                if (offset > 0) {
                    String part(buffer, offset);
                    results.Move(std::move(part));
                }

                buffer = &buffer[offset + sepLength];
            }
        } while (ptr != nullptr);

        if (*buffer != END) {
            String part(buffer);
            results.Move(std::move(part));
        }
    }

    String String::SubString(uint32 from, uint32 length) const {
        if (from >= GetLength())
            return "";

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

    template<typename Callable>
    String Convert(const String& source, Callable&& callable) {
        auto len = source.GetLength();
        auto data = source.GetStr_C();

        String result(len + 1);
        auto out = result.GetStr_C();
        auto outLen = len;

        while (len > 0) {
            Unicode::Char32u code;
            uint32 toOffset = len;
            auto status = Unicode::Utf8toUtf32((const Unicode::Char8u*) data, toOffset, code);

            if (!status) {
                return "";
            }

            code = callable(code);

            uint32 outToOffset = 0;
            status = Unicode::Utf32toUtf8(code, (Unicode::Char8u*) out, outToOffset);

            if (!status) {
                return "";
            }

            out = out + outToOffset;
            data = data + toOffset;

            outLen -= outToOffset;
            len -= toOffset;

            assert(len == outLen);
        }

        *out = String::END;
        return result;
    }

    String String::ToLower() const {
        return Convert(*this, [](Unicode::Char32u ch){ return Unicode::ToLower(ch); });
    }

    String String::ToUpper() const {
        return Convert(*this, [](Unicode::Char32u ch){ return Unicode::ToUpper(ch); });
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

    String String::From(int value) {
        Char8u buffer[64];
        snprintf(buffer, 64, "%i", value);
        return buffer;
    }

    String String::From(unsigned int value) {
        Char8u buffer[128];
        snprintf(buffer, 128, "%u", value);
        return buffer;
    }

    String String::From(long value) {
        Char8u buffer[64];
        snprintf(buffer, 64, "%li", value);
        return buffer;
    }

    String String::From(unsigned long value) {
        Char8u buffer[128];
        snprintf(buffer, 128, "%lu", value);
        return buffer;
    }

    String String::Fromf(float value, uint32 precision) {
        Char8u buffer[64];
        snprintf(buffer, 64, "%.*f", precision, value);
        return buffer;
    }

    String String::Fromd(double value, uint32 precision) {
        Char8u buffer[128];
        snprintf(buffer, 128, "%.*lf", precision, value);
        return buffer;
    }

    String String::Fromi32(int32 value) {
        Char8u buffer[64];
        snprintf(buffer, 64, "%i", value);
        return buffer;
    }

    String String::Fromi64(int64 value) {
        Char8u buffer[128];
        snprintf(buffer, 128, "%lli", (long long int) value);
        return buffer;
    }

    String String::Fromu32(uint32 value) {
        Char8u buffer[64];
        snprintf(buffer, 64, "%u", value);
        return buffer;
    }

    String String::Fromu64(uint64 value) {
        Char8u buffer[128];
        snprintf(buffer, 128, "%llu", (unsigned long long int) value);
        return buffer;
    }

    String String::Fromb(bool value) {
        return value? BERSERK_TEXT("True"): BERSERK_TEXT("False");
    }

    String String::Fromp(const void *value) {
        Char8u buffer[128];
        snprintf(buffer, 128, "%p", value);
        return buffer;
    }

    void *String::AllocateBuffer(uint32 capacity) {
        return System::Impl::Instance().AllocateStringBuffer(capacity * sizeof(Char8u));
    }

    void String::DeallocateBuffer(Char8u *memory, size_t capacity) {
        System::Impl::Instance().DeallocateStringBuffer(memory, capacity * sizeof(Char8u));
    }

    void String::AlignCapacity(uint32 &capacity) {
        // Can assign what ever we want
        // It would be better to set as min the cache line size
        uint32 bytesSize = 64;
        uint32 factor = 2;

        while (bytesSize < capacity * sizeof(Char8u)) {
            bytesSize *= factor;
        }

        capacity = bytesSize / sizeof(Char8u);
    }

    void String::StoreString(const String::Char8u *str, uint32 length) {
        if (length < CONST_BUFFER_SIZE) {
            Memory::Copy(mStatic, str, length * sizeof(Char8u));
        } else {
            mCapacity = length + 1;
            AlignCapacity(mCapacity);
            mDynamic = (Char8u *) AllocateBuffer(mCapacity);
            Memory::Copy(mDynamic, str, length * sizeof(Char8u));
        }

        GetStr_C()[length] = END;
    }

    void String::AppendAndStoreString(const String::Char8u *str, uint32 length) {
        uint32 myLength = GetLength();
        uint32 newLength = myLength + length;

        if (newLength < GetCapacity()) {
            Memory::Copy(GetStr_C() + myLength, str, length * sizeof(Char8u));
        } else {
            uint32 newCapacity = newLength + 1;
            AlignCapacity(newCapacity);
            auto *newDynamic = (Char8u *) AllocateBuffer(newCapacity);

            Memory::Copy(newDynamic, GetStr_C(), myLength * sizeof(Char8u));
            Memory::Copy(newDynamic + myLength, str, length * sizeof(Char8u));

            if (IsDynamic()) {
                DeallocateBuffer(mDynamic, GetCapacity());
            }

            mCapacity = newCapacity;
            mDynamic = newDynamic;
        }

        GetStr_C()[newLength] = END;
    }

    void String::ConcatenateAndStoreStrings(const String::Char8u *str1, uint32 len1, const String::Char8u *str2, uint32 len2) {
        uint32 newLength = len1 + len2;

        if (newLength < GetCapacity()) {
            Memory::Copy(GetStr_C(), str1, len1 * sizeof(Char8u));
            Memory::Copy(GetStr_C() + len1, str2, len2 * sizeof(Char8u));
        } else {
            uint32 newCapacity = newLength + 1;
            AlignCapacity(newCapacity);
            Char8u *newDynamic = (Char8u *) AllocateBuffer(newCapacity);

            Memory::Copy(newDynamic, str1, len1 * sizeof(Char8u));
            Memory::Copy(newDynamic + len1, str2, len2 * sizeof(Char8u));

            if (IsDynamic()) {
                DeallocateBuffer(mDynamic, GetCapacity());
            }

            mCapacity = newCapacity;
            mDynamic = newDynamic;
        }

        GetStr_C()[newLength] = END;
    }

}