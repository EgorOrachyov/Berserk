/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/String/String.hpp>
#include <BerserkCore/Application.hpp>

namespace Berserk {
    namespace Details {
        void *AllocateString(uint32 size) {
            // May be replaced later with some pool for strings allocation
            Allocator *allocator = Application::GetSingleton().GetGlobalAllocator();
            return allocator ? allocator->Allocate(size) : nullptr;
        }

        void FreeString(void *block) {
            Allocator *allocator = Application::GetSingleton().GetGlobalAllocator();
            if (allocator) allocator->Free(block);
        }

        void AlignString(uint32 &capacity, uint32 base) {
            // Can assign what ever we want
            // It would be better to set as min the cache line size
            uint32 bytesSize = 64;
            uint32 factor = 2;

            while (bytesSize < capacity * base) {
                bytesSize *= factor;
            }

            capacity = bytesSize / base;
        }
    }

    String::String(const String::CHAR *str)
            : String(str, Utils::Length(str)) {

    }

    String::String(uint32 capacity) {
        if (capacity > CONST_BUFFER_SIZE) {
            mCapacity = capacity;
            Details::AlignString(mCapacity, sizeof(CHAR));
            mDynamic = (CHAR *) Details::AllocateString(mCapacity * sizeof(CHAR));
            mDynamic[0] = END;
        }
    }

    String::String(const String::CHAR *str, uint32 length) {
        StoreString(str, length);
    }

    String::String(const String &str) {
        StoreString(str.GetStr(), str.GetLength());
    }

    String::String(String &&str) noexcept {
        if (str.IsStatic()) {
            Memory::Copy(mStatic, str.mStatic, (str.GetLength() + 1) * sizeof(CHAR));
            str.mStatic[0] = END;
        } else {
            mCapacity = str.mCapacity;
            mDynamic = str.mDynamic;
            str.mCapacity = 0;
            str.mDynamic = nullptr;
        }
    }

    String::~String() {
        if (IsDynamic())
            Details::FreeString(mDynamic);

        mDynamic = nullptr;
        mCapacity = 0;
    }

    String& String::operator=(const String &other) {
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
        AddAndStoreString(other.GetStr(), other.GetLength());
        return *this;
    }

    String &String::operator+=(const String &other) {
        AddAndStoreString(other.GetStr(), other.GetLength());
        return *this;
    }

    String String::operator+(const String &other) const {
        String result;
        result.AddAndStoreStrings(GetStr(), GetLength(), other.GetStr(), other.GetLength());
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

    int32 String::ToUint32() const {
        const int32 BASE = 10;
        return (uint32) strtoull(GetStr(), nullptr, BASE);
    }

    int64 String::ToUint64() const {
        const int32 BASE = 10;
        return (uint64) strtoull(GetStr(), nullptr, BASE);
    }

    String String::ToString(float value, uint32 precision) {
        CHAR buffer[64];
        snprintf(buffer, 64, "%.*f", precision, value);
        return buffer;
    }

    String String::ToString(double value, uint32 precision) {
        CHAR buffer[128];
        snprintf(buffer, 128, "%.*lf", precision, value);
        return buffer;
    }

    String String::ToString(int32 value) {
        CHAR buffer[64];
        snprintf(buffer, 64, "%.i", value);
        return buffer;
    }

    String String::ToString(int64 value) {
        CHAR buffer[128];
        snprintf(buffer, 128, "%.lli", (long long int) value);
        return buffer;
    }

    String String::ToString(uint32 value) {
        CHAR buffer[64];
        snprintf(buffer, 64, "%.u", value);
        return buffer;
    }

    String String::ToString(uint64 value) {
        CHAR buffer[128];
        snprintf(buffer, 128, "%.llu", (unsigned long long int) value);
        return buffer;
    }

    void String::StoreString(const String::CHAR *str, uint32 length) {
        if (length < CONST_BUFFER_SIZE) {
            Memory::Copy(mStatic, str, length * sizeof(CHAR));
        } else {
            mCapacity = length + 1;
            Details::AlignString(mCapacity, sizeof(CHAR));
            mDynamic = (CHAR *) Details::AllocateString(mCapacity * sizeof(CHAR));
            Memory::Copy(mDynamic, str, length * sizeof(CHAR));
        }

        GetStr()[length] = END;
    }

    void String::AddAndStoreString(const String::CHAR *str, uint32 length) {
        uint32 myLength = GetLength();
        uint32 newLength = myLength + length;

        if (newLength < GetCapacity()) {
            Memory::Copy(GetStr() + myLength, str, length * sizeof(CHAR));
        } else {
            uint32 newCapacity = newLength + 1;
            Details::AlignString(newCapacity, sizeof(CHAR));
            CHAR *newDynamic = (CHAR *) Details::AllocateString(newCapacity * sizeof(CHAR));

            Memory::Copy(newDynamic, GetStr(), myLength * sizeof(CHAR));
            Memory::Copy(newDynamic + myLength, str, length * sizeof(CHAR));

            if (IsDynamic()) {
                Details::FreeString(mDynamic);
            }

            mCapacity = newCapacity;
            mDynamic = newDynamic;
        }

        GetStr()[newLength] = END;
    }

    void String::AddAndStoreStrings(const String::CHAR *str1, uint32 len1, const String::CHAR *str2, uint32 len2) {
        uint32 newLength = len1 + len2;

        if (newLength < GetCapacity()) {
            Memory::Copy(GetStr(), str1, len1 * sizeof(CHAR));
            Memory::Copy(GetStr() + len1, str2, len2 * sizeof(CHAR));
        } else {
            uint32 newCapacity = newLength + 1;
            Details::AlignString(newCapacity, sizeof(CHAR));
            CHAR *newDynamic = (CHAR *) Details::AllocateString(newCapacity * sizeof(CHAR));

            Memory::Copy(newDynamic, str1, len1 * sizeof(CHAR));
            Memory::Copy(newDynamic + len1, str2, len2 * sizeof(CHAR));

            if (IsDynamic()) {
                Details::FreeString(mDynamic);
            }

            mCapacity = newCapacity;
            mDynamic = newDynamic;
        }

        GetStr()[newLength] = END;
    }

}