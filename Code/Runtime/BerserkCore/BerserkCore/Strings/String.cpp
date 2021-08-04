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
            mDynamic = (CharType *) AllocateBuffer(mCapacity);
            mDynamic[0] = END;
        }
    }

    String::String(const CharType *str) {
        assert(str);

        if (str) {
            StoreString(str, Utils::Length(str));
        }
    }

    String::String(const String::CharType *str, uint32 length) {
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
        CharType buffer[2] = {character, END };
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
        CharType buffer[64];
        snprintf(buffer, 64, "%i", value);
        return buffer;
    }

    String String::From(unsigned int value) {
        CharType buffer[128];
        snprintf(buffer, 128, "%u", value);
        return buffer;
    }

    String String::From(long value) {
        CharType buffer[64];
        snprintf(buffer, 64, "%li", value);
        return buffer;
    }

    String String::From(unsigned long value) {
        CharType buffer[128];
        snprintf(buffer, 128, "%lu", value);
        return buffer;
    }

    String String::Fromf(float value, uint32 precision) {
        CharType buffer[64];
        snprintf(buffer, 64, "%.*f", precision, value);
        return buffer;
    }

    String String::Fromd(double value, uint32 precision) {
        CharType buffer[128];
        snprintf(buffer, 128, "%.*lf", precision, value);
        return buffer;
    }

    String String::Fromi32(int32 value) {
        CharType buffer[64];
        snprintf(buffer, 64, "%i", value);
        return buffer;
    }

    String String::Fromi64(int64 value) {
        CharType buffer[128];
        snprintf(buffer, 128, "%lli", (long long int) value);
        return buffer;
    }

    String String::Fromu32(uint32 value) {
        CharType buffer[64];
        snprintf(buffer, 64, "%u", value);
        return buffer;
    }

    String String::Fromu64(uint64 value) {
        CharType buffer[128];
        snprintf(buffer, 128, "%llu", (unsigned long long int) value);
        return buffer;
    }

    String String::Fromb(bool value) {
        return value? BERSERK_TEXT("True"): BERSERK_TEXT("False");
    }

    String String::Fromp(const void *value) {
        CharType buffer[128];
        snprintf(buffer, 128, "%p", value);
        return buffer;
    }
}