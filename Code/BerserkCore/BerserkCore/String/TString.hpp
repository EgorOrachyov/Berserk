/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TSTRING_HPP
#define BERSERK_TSTRING_HPP

#include <BerserkCore/String/TStringUtils.hpp>
#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/Crc32.h>

namespace Berserk {

    namespace Details {
        void* AllocateString(uint32 sizeInBytes);
        void FreeString(void* block);
        void AlignString(uint32 &capacity, uint32 base);
    }

    template <typename T, T end, uint32 ConstBufferSize>
    class TString {
    public:

        static const uint32 STATIC_CAPACITY = ConstBufferSize;
        using Utils = TStringUtils<T,end>;
        using Type = TString<T,end,ConstBufferSize>;

        TString() = default;

        TString(const T* str)
            : TString(str, Utils::Length(str)) {

        }

        TString(uint32 capacity) {
            if (capacity > ConstBufferSize) {
                mCapacity = capacity;
                Details::AlignString(mCapacity, sizeof(T));
                mDynamic = (T*) Details::AllocateString(mCapacity * sizeof(T));
                mDynamic[0] = end;
            }
        }

        TString(const T* str, uint32 length) {
            StoreString(str, length);
        }

        TString(const TString& str) {
            StoreString(str.GetStr(), str.GetLength());
        }

        TString(TString&& str) noexcept {
            if (str.IsStatic()) {
                Memory::Copy(mStatic, str.mStatic, (str.GetLength() + 1) * sizeof(T));
                str.mStatic[0] = end;
            }
            else {
                mCapacity = str.mCapacity;
                mDynamic = str.mDynamic;
                str.mCapacity = 0;
                str.mDynamic = nullptr;
            }
        }

        ~TString() {
            if (IsDynamic())
                Details::FreeString(mDynamic);

            mDynamic = nullptr;
            mCapacity = 0;
        }

        TString& operator=(const TString& other) {
            if (this == &other) {
                return *this;
            }

            this->~TString();
            new (this) TString(other);
            return *this;
        }

        TString& operator=(TString&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~TString();
            new (this) TString(std::move(other));
            return *this;
        }

        TString& Add(const TString& other) {
            AddAndStoreString(other.GetStr(), other.GetLength());
            return *this;
        }

        TString& operator+=(const TString& other) {
            AddAndStoreString(other.GetStr(),other.GetLength());
            return *this;
        }

        TString operator+(const TString& other) const {
            TString result;
            result.AddAndStoreStrings(GetStr(), GetLength(), other.GetStr(), other.GetLength());
            return result;
        }

        bool operator==(const TString& other) const {
            return Utils::Compare(GetStr(), other.GetStr()) == 0;
        }

        bool operator!=(const TString& other) const {
            return Utils::Compare(GetStr(), other.GetStr()) != 0;
        }

        bool operator<=(const TString& other) const {
            return Utils::Compare(GetStr(), other.GetStr()) <= 0;
        }

        bool operator>=(const TString& other) const {
            return Utils::Compare(GetStr(), other.GetStr()) >= 0;
        }

        bool operator<(const TString& other) const {
            return Utils::Compare(GetStr(), other.GetStr()) < 0;
        }

        bool operator>(const TString& other) const {
            return Utils::Compare(GetStr(), other.GetStr()) > 0;
        }

        uint32 GetCapacity() const {
            return IsStatic()? ConstBufferSize: mCapacity;
        }

        uint32 GetLength() const {
            return Utils::Length(GetStr());
        }

        const T* GetStr() const {
            return IsStatic()? mStatic: mDynamic;
        }

        T* GetStr() {
            return IsStatic()? mStatic: mDynamic;
        }

        uint32 Hash() const {
            return Crc32::Hash(GetStr(), GetLength());
        }

    private:

        bool IsStatic() const { return mCapacity == 0; }
        bool IsDynamic() const { return mCapacity != 0; }

        void StoreString(const T* str, uint32 length) {
            if (length < ConstBufferSize) {
                Memory::Copy(mStatic, str, length * sizeof(T));
            }
            else {
                mCapacity = length + 1;
                Details::AlignString(mCapacity, sizeof(T));
                mDynamic = (T*) Details::AllocateString(mCapacity * sizeof(T));
                Memory::Copy(mDynamic, str, length * sizeof(T));
            }

            GetStr()[length] = end;
        }

        void AddAndStoreString(const T* str, uint32 length) {
            uint32 myLength = GetLength();
            uint32 newLength = myLength + length;

            if (newLength < GetCapacity()) {
                Memory::Copy(GetStr() + myLength, str, length * sizeof(T));
            }
            else {
                uint32 newCapacity = newLength + 1;
                Details::AlignString(newCapacity, sizeof(T));
                T* newDynamic = (T*) Details::AllocateString(newCapacity * sizeof(T));

                Memory::Copy(newDynamic, GetStr(), myLength * sizeof(T));
                Memory::Copy(newDynamic + myLength, str, length * sizeof(T));

                if (IsDynamic()) {
                    Details::FreeString(mDynamic);
                }

                mCapacity = newCapacity;
                mDynamic = newDynamic;
            }

            GetStr()[newLength] = end;
        }

        void AddAndStoreStrings(const T* str1, uint32 len1, const T* str2, uint32 len2) {
            uint32 newLength = len1 + len2;

            if (newLength < GetCapacity()) {
                Memory::Copy(GetStr(), str1, len1 * sizeof(T));
                Memory::Copy(GetStr() + len1, str2, len2 * sizeof(T));
            }
            else {
                uint32 newCapacity = newLength + 1;
                Details::AlignString(newCapacity, sizeof(T));
                T* newDynamic = (T*) Details::AllocateString(newCapacity * sizeof(T));

                Memory::Copy(newDynamic, str1, len1 * sizeof(T));
                Memory::Copy(newDynamic + len1, str2, len2 * sizeof(T));

                if (IsDynamic()) {
                    Details::FreeString(mDynamic);
                }

                mCapacity = newCapacity;
                mDynamic = newDynamic;
            }

            GetStr()[newLength] = end;
        }

        uint32 mCapacity = 0;

        union {
            T* mDynamic;
            T mStatic[ConstBufferSize] = { end };
        };
    };

    namespace Details {
        static constexpr uint32 STRING_CHAR_CAPACITY = 28/sizeof(char);
        static constexpr uint32 STRING_WCHAR_CAPACITY = 28/sizeof(wchar);
    }

    using String = TString<char,'\0',Details::STRING_CHAR_CAPACITY>;
    using WString = TString<wchar,L'\0',Details::STRING_WCHAR_CAPACITY>;

}

#endif //BERSERK_TSTRING_HPP