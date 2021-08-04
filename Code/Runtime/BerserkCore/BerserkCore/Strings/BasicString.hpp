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

#ifndef BERSERK_BASICSTRING_HPP
#define BERSERK_BASICSTRING_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Templates/Contracts.hpp>
#include <BerserkCore/Strings/StringUtils.hpp>
#include <BerserkCore/Platform/Memory.hpp>

namespace Berserk {

    class BasicStringDetails {
    public:
        static void *AllocateBuffer(uint64 size);
        static void DeallocateBuffer(void *memory, uint64 size);
    };

    template <typename CharT, CharT End, uint32 BufferSize>
    class BasicString {
    public:
        /** Small buffer capacity */
        static const uint32 CONST_BUFFER_SIZE = BufferSize;
        /** Null terminator */
        static const CharT END = End;
        /** Char unit */
        using CharType = CharT;
        /** String utility for char unit */
        using Utils = TStringUtils<CharType, END>;

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

        BasicString() = default;

        BasicString(const CharType *str) {
            assert(str);

            if (str) {
                StoreString(str, Utils::Length(str));
            }
        }

        BasicString(uint32 capacity) {
            if (capacity > CONST_BUFFER_SIZE) {
                mCapacity = capacity;
                AlignCapacity(mCapacity);
                mDynamic = (CharType *) AllocateBuffer(mCapacity);
                mDynamic[0] = END;
            }
        }

        BasicString(const CharType *str, uint32 length) {
            assert(str);

            if (str) {
                StoreString(str, length);
            }
        }

        BasicString(const BasicString &str) {
            StoreString(str.GetStr_C(), str.GetLength());
        }

        BasicString(BasicString &&str) noexcept {
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

        ~BasicString() {
            if (IsDynamic()) {
                DeallocateBuffer(mDynamic, GetCapacity());
            }

            mDynamic = nullptr;
            mCapacity = 0;
        }

        /** @return lexicographical '==' */
        bool operator==(const BasicString &other) const {
            return Utils::Compare(GetStr_C(), other.GetStr_C()) == 0;
        }

        /** @return lexicographical '!=' */
        bool operator!=(const BasicString &other) const {
            return Utils::Compare(GetStr_C(), other.GetStr_C()) != 0;
        }

        Result FindFirst(const CharType* substring) const {
            auto ptr = Utils::FindFirst(GetStr_C(), substring);
            return ptr ? Result(GetOffsetOf(ptr)) : Result();
        }

        Result FindLast(const CharType* substring) const {
            auto ptr = Utils::FindLast(GetStr_C(), substring);
            return ptr ? Result(GetOffsetOf(ptr)) : Result();
        }

        /** @return BasicString max CharType units capacity (including null-terminator) */
        uint32 GetCapacity() const { return IsStatic() ? CONST_BUFFER_SIZE : mCapacity; }

        /** @return BasicString length in CharType units */
        uint32 GetLength() const { return Utils::Length(GetStr_C()); }

        /** @return C compatible null-terminated string buffer */
        const CharType *GetStr_C() const { return IsStatic() ? mStatic : mDynamic; }

        /** @return C compatible null-terminated string buffer */
        CharType *GetStr_C() { return IsStatic() ? mStatic : mDynamic; }

        /** @return Hash of the string content */
        uint32 Hash() const {
            return Crc32::Hash(GetStr_C(), GetLength());
        }

    protected:
        bool IsStatic() const { return mCapacity == 0; }
        bool IsDynamic() const { return mCapacity != 0; }

        uint32 GetOffsetOf(const char* ptr) const { return GetOffsetOf(GetStr_C(), ptr); }
        static uint32 GetOffsetOf(const char* source, const char* ptr) { return (uint32)((uint64)ptr - (uint64)source); }

        void *AllocateBuffer(uint32 capacity) {
            return BasicStringDetails::AllocateBuffer(capacity * sizeof(CharType));
        }

        void DeallocateBuffer(CharType *memory, uint64 capacity) {
            BasicStringDetails::DeallocateBuffer(memory, capacity * sizeof(CharType));
        }

        void AlignCapacity(uint32 &capacity) {
            // Can assign what ever we want
            // It would be better to set as min the cache line size
            uint32 bytesSize = 64;
            uint32 factor = 2;

            while (bytesSize < capacity * sizeof(CharType)) {
                bytesSize *= factor;
            }

            capacity = bytesSize / sizeof(CharType);
        }

        void StoreString(const CharType *str, uint32 length) {
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

        void AppendAndStoreString(const CharType *str, uint32 length) {
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

        void ConcatenateAndStoreStrings(const CharType *str1, uint32 len1, const CharType *str2, uint32 len2) {
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

    protected:
        uint32 mCapacity = 0;

        union {
            CharType *mDynamic;
            CharType mStatic[CONST_BUFFER_SIZE] = {END};
        };
    };

}

#endif //BERSERK_BASICSTRING_HPP
