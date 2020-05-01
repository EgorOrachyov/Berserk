/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TSTRING_H
#define BERSERK_TSTRING_H

#include <Crc32.h>
#include <AllocPool.h>
#include <Platform/Memory.h>
#include <String/TStringUtility.h>

namespace Berserk {

    /**
     * Allocates string buffers of power of 2 size. Min size 64.
     * Uses internally memory pools to optimize memory allocation.
     */
    class StringBufferAlloc {
    private:
        StringBufferAlloc();
        ~StringBufferAlloc();
        void* allocate(uint32 &size);
        void free(void* memory, uint32 size);
        void expand(uint32 index);
        uint32 getIndex(uint32 size) const;

        template <typename T>
        void* allocateT(uint32& capacity) {
            uint32 size = capacity * sizeof(T);
            void* mem = allocate(size);
            capacity = size / sizeof(T);
            return mem;
        }

        template <typename T>
        void freeT(void* memory, uint32 capacity) {
            uint32 size = capacity * sizeof(T);
            free(memory, size);
        }
    private:
        static const uint32 POOL_SIZE_FACTOR;
        static const uint32 POOL_SIZE_INITIAL;
        static StringBufferAlloc& getSingleton();
    private:
        template <typename Char, Char end, uint32 SMALL_BUFFER>
        friend class TString;
        friend class CString;
        friend class WString;
    };

    /**
     * Template string of dynamic size.
     * Uses small string buffer optimization to store strings of small size without
     * dynamic allocation of the buffer. Could be used to store string data from any thread.
     * @note Concrete string could be accessed only by one thread at once.
     * @tparam Char Type of stored char symbols
     * @tparam end Char marker of the string end (for C compatibility)
     * @tparam SMALL_BUFFER Internall small buffer capacity
     */
    template <typename Char = char, Char end = '\0', uint32 SMALL_BUFFER = 24>
    class TString {
    public:
        static const uint32 SMALL_BUFFER_SIZE = SMALL_BUFFER;
        using Util = TStringUtility<Char, end>;

        TString() = default;
        TString(const Char* buffer) {
            auto length = Util::length(buffer);
            auto capacity = length + 1;
            if (length < SMALL_BUFFER_SIZE) {
                Memory::copy(small, buffer, sizeof(Char) * capacity);
            }
            else {
                mCapacity = capacity;
                allocated = (Char*) StringBufferAlloc::getSingleton().allocateT<Char>(mCapacity);
                Memory::copy(allocated, buffer, sizeof(Char) * capacity);
            }
        }
        TString(const Char* buffer, uint32 length) {
            if (length < SMALL_BUFFER_SIZE) {
                Memory::copy(small, buffer, sizeof(Char) * length);
            }
            else {
                mCapacity = length + 1;
                allocated = (Char*) StringBufferAlloc::getSingleton().allocateT<Char>(mCapacity);
                Memory::copy(allocated, buffer, sizeof(Char) * length);
            }
            data()[length] = '\0';
        }
        TString(const TString& other) {
            auto length = other.length();
            auto capacity = length + 1;
            if (length < SMALL_BUFFER_SIZE) {
                Memory::copy(small, other.data(), sizeof(Char) * capacity);
            }
            else {
                mCapacity = length + 1;
                allocated = (Char*) StringBufferAlloc::getSingleton().allocateT<Char>(mCapacity);
                Memory::copy(allocated, other.data(), sizeof(Char) * capacity);
            }
        }
        template <uint32 N>
        TString(const TString<Char,end,N> &other) {
            auto length = other.length();
            auto capacity = length + 1;
            if (length < SMALL_BUFFER_SIZE) {
                Memory::copy(small, other.data(), sizeof(Char) * capacity);
            }
            else {
                mCapacity = length + 1;
                allocated = (Char*) StringBufferAlloc::getSingleton().allocateT<Char>(mCapacity);
                Memory::copy(allocated, other.data(), sizeof(Char) * capacity);
            }
        }
        TString(TString&& other) noexcept {
            mCapacity = other.mCapacity;

            if (other.isStatic()) {
                Memory::copy(small, other.small, sizeof(Char) * SMALL_BUFFER_SIZE);
            }
            else {
                allocated = other.allocated;
            }

            other.mCapacity = 0;
            other.small[0] = end;
        }
        ~TString() {
            if (!isStatic()) {
                StringBufferAlloc::getSingleton().freeT<Char>(allocated, mCapacity);
            }

            mCapacity = 0;
            small[0] = end;
        }

        void ensureCapacity(uint32 desired) {
            if (capacity() < desired) {
                auto newCapacity = desired;
                auto newAllocated = (Char*) StringBufferAlloc::getSingleton().allocateT<Char>(newCapacity);
                Memory::copy(newAllocated, data(), sizeof(Char) * (length() + 1));

                this->~TString();

                mCapacity = newCapacity;
                allocated = newAllocated;
            }
        }

        TString& fromBuffer(const Char* buffer, uint32 length) {
            this->~TString();
            auto capacity = length + 1;
            if (length < SMALL_BUFFER_SIZE) {
                Memory::copy(small, buffer, sizeof(Char) * length);
            }
            else {
                mCapacity = length + 1;
                allocated = (Char*) StringBufferAlloc::getSingleton().allocateT<Char>(mCapacity);
                Memory::copy(allocated, buffer, sizeof(Char) * length);
            }
            data()[length] = '\0';

            return *this;
        }

        TString& operator=(const TString& other) {
            this->~TString();
            auto length = other.length();
            auto capacity = length + 1;
            if (length < SMALL_BUFFER_SIZE) {
                Memory::copy(small, other.data(), sizeof(Char) * capacity);
            }
            else {
                mCapacity = length + 1;
                allocated = (Char*) StringBufferAlloc::getSingleton().allocateT<Char>(mCapacity);
                Memory::copy(allocated, other.data(), sizeof(Char) * capacity);
            }

            return *this;
        }
        template <uint32 N>
        TString& operator=(const TString<Char,end,N>& other) {
            this->~TString();
            auto length = other.length();
            auto capacity = length + 1;
            if (length < SMALL_BUFFER_SIZE) {
                Memory::copy(small, other.data(), sizeof(Char) * capacity);
            }
            else {
                mCapacity = length + 1;
                allocated = (Char*) StringBufferAlloc::getSingleton().allocateT<Char>(mCapacity);
                Memory::copy(allocated, other.data(), sizeof(Char) * capacity);
            }

            return *this;
        }
        TString& operator=(TString&& other) noexcept {
            this->~TString();

            mCapacity = other.mCapacity;

            if (other.isStatic()) {
                Memory::copy(small, other.small, sizeof(Char) * SMALL_BUFFER_SIZE);
            }
            else {
                allocated = other.allocated;
            }

            other.mCapacity = 0;
            other.small[0] = end;

            return *this;
        }
        TString& operator=(const Char* buffer) {
            this->~TString();
            auto length = Util::length(buffer);
            auto capacity = length + 1;
            if (length < SMALL_BUFFER_SIZE) {
                Memory::copy(small, buffer, sizeof(Char) * capacity);
            }
            else {
                mCapacity = capacity;
                allocated = (Char*) StringBufferAlloc::getSingleton().allocateT<Char>(mCapacity);
                Memory::copy(allocated, buffer, sizeof(Char) * capacity);
            }

            return *this;
        }

        TString& operator+=(const TString& other) {
            auto mlen = length();
            auto olen = other.length();
            auto osize = olen + 1;

            if (capacity() > (mlen + olen)) {
                Memory::copy(data() + mlen, other.data(), sizeof(Char) * osize);
            }
            else {
                uint32 newCapacity = mlen + olen + 1;
                Char* newBuffer = (Char*)StringBufferAlloc::getSingleton().allocateT<Char>(newCapacity);
                Memory::copy(newBuffer, data(), sizeof(Char) * mlen);
                Memory::copy(newBuffer + mlen, other.data(), sizeof(Char) * osize);

                if (!isStatic()) {
                    StringBufferAlloc::getSingleton().freeT<Char>(allocated, mCapacity);
                }

                allocated = newBuffer;
                mCapacity = newCapacity;
            }

            return *this;
        }
        TString& operator+=(const Char* buffer) {
            auto mlen = length();
            auto olen = Util::length(buffer);
            auto osize = olen + 1;

            if (capacity() > (mlen + olen)) {
                Memory::copy(data() + mlen, buffer, sizeof(Char) * osize);
            }
            else {

                uint32 newCapacity = mlen + olen + 1;
                Char* newBuffer = (Char*)StringBufferAlloc::getSingleton().allocateT<Char>(newCapacity);
                Memory::copy(newBuffer, data(), sizeof(Char) * mlen);
                Memory::copy(newBuffer + mlen, buffer, sizeof(Char) * osize);

                if (!isStatic()) {
                    StringBufferAlloc::getSingleton().freeT<Char>(allocated, mCapacity);
                }

                allocated = newBuffer;
                mCapacity = newCapacity;
            }

            return *this;
        }

        TString operator+(const TString& other) const {
            TString result;

            auto mlen = length();
            auto olen = other.length();
            auto osize = olen + 1;

            if (SMALL_BUFFER_SIZE > (mlen + olen)) {
                Memory::copy(result.small, data(), sizeof(Char) * mlen);
                Memory::copy(result.small + mlen, other.data(), sizeof(Char) * osize);
            }
            else {
                result.mCapacity = mlen + olen + 1;
                result.allocated = (Char*)StringBufferAlloc::getSingleton().allocateT<Char>(result.mCapacity);
                Memory::copy(result.allocated, data(), sizeof(Char) * mlen);
                Memory::copy(result.allocated + mlen, other.data(), sizeof(Char) * osize);
            }

            return result;
        }
        TString operator+(const Char* buffer) const {
            TString result;

            auto mlen = length();
            auto olen = Util::length(buffer);
            auto osize = olen + 1;

            if (SMALL_BUFFER_SIZE > (mlen + olen)) {
                Memory::copy(result.small, data(), sizeof(Char) * mlen);
                Memory::copy(result.small + mlen, buffer, sizeof(Char) * osize);
            }
            else {
                result.mCapacity = mlen + olen + 1;
                result.allocated = (Char*)StringBufferAlloc::getSingleton().allocateT<Char>(result.mCapacity);
                Memory::copy(result.allocated, data(), sizeof(Char) * mlen);
                Memory::copy(result.allocated + mlen, buffer, sizeof(Char) * osize);
            }

            return result;
        }

        bool operator==(const TString& other) const {
            return Util::compare(data(), other.data()) == 0;
        }
        bool operator!=(const TString& other) const {
            return Util::compare(data(), other.data()) != 0;
        }
        bool operator<=(const TString& other) const {
            return Util::compare(data(), other.data()) <= 0;
        }
        bool operator>=(const TString& other) const {
            return Util::compare(data(), other.data()) >= 0;
        }
        bool operator<(const TString& other) const {
            return Util::compare(data(), other.data()) < 0;
        }
        bool operator>(const TString& other) const {
            return Util::compare(data(), other.data()) > 0;
        }

        bool operator==(const Char* other) const {
            return Util::compare(data(), other) == 0;
        }
        bool operator<=(const Char* other) const {
            return Util::compare(data(), other) <= 0;
        }
        bool operator>=(const Char* other) const {
            return Util::compare(data(), other) >= 0;
        }
        bool operator<(const Char* other) const {
            return Util::compare(data(), other) < 0;
        }
        bool operator>(const Char* other) const {
            return Util::compare(data(), other) > 0;
        }

        TString operator*(uint32 N) {
            TString result;

            auto mlen = length();
            auto rlen = mlen * N;
            auto rsize = rlen + 1;

            if (SMALL_BUFFER_SIZE <= rlen) {
                result.mCapacity = rsize;
                result.allocated = (Char*)StringBufferAlloc::getSingleton().allocateT<Char>(result.mCapacity);
            }

            Util::repeat(result.data(), rsize, data(), N);
            return result;
        }

        bool replaceFirst(const Char* substring, const Char* replacement) {
            const Char* find = findFirst(substring);

            if (find != nullptr) {
                auto slen = Util::length(substring);
                auto rlen = Util::length(replacement);
                auto first = Util::offset(data(), find);

                if (slen >= rlen) {
                    Util::replace(data(), capacity(), data(), first, slen, replacement);
                }
                else {
                    auto len = length() - slen + rlen;
                    auto size = len + 1;

                    if (len < SMALL_BUFFER_SIZE) {
                        Char tmp[SMALL_BUFFER_SIZE];
                        Util::replace(tmp, SMALL_BUFFER_SIZE, data(), first, slen, replacement);
                        *this = tmp;
                    }
                    else {
                        Char* tmp = (Char*)StringBufferAlloc::getSingleton().allocateT<Char>(size);
                        Util::replace(tmp, size, data(), first, slen, replacement);

                        this->~TString();
                        mCapacity = size;
                        allocated = tmp;
                    }
                }
            }

            return find != nullptr;
        }
        bool replaceLast(const Char* substring, const Char* replacement) {
            const Char* find = findLast(substring);

            if (find != nullptr) {
                auto slen = Util::length(substring);
                auto rlen = Util::length(replacement);
                auto first = Util::offset(data(), find);

                if (slen >= rlen) {
                    Util::replace(data(), capacity(), data(), first, slen, replacement);
                }
                else {
                    auto len = length() - slen + rlen;
                    auto size = len + 1;

                    if (len < SMALL_BUFFER_SIZE) {
                        Char tmp[SMALL_BUFFER_SIZE];
                        Util::replace(tmp, SMALL_BUFFER_SIZE, data(), first, slen, replacement);
                        *this = tmp;
                    }
                    else {
                        Char* tmp = (Char*)StringBufferAlloc::getSingleton().allocateT<Char>(size);
                        Util::replace(tmp, size, data(), first, slen, replacement);

                        this->~TString();
                        mCapacity = size;
                        allocated = tmp;
                    }
                }
            }

            return find != nullptr;
        }

        Char* findFirst(const TString& other) {
            return (Char*) Util::findFirst(data(), other.data());
        }
        const Char* findFirst(const TString& other) const {
            return Util::findFirst(data(), other.data());
        }
        Char* findLast(const TString& other) {
            return (Char*) Util::findLast(data(), other.data());
        }
        const Char* findLast(const TString& other) const {
            return Util::findLast(data(), other.data());
        }

        Char* data() { return (mCapacity == 0? small : allocated); }
        const Char* data() const { return (mCapacity == 0? small : allocated); }

        bool isStatic() const { return mCapacity == 0; }
        uint32 length() const { return Util::length(data()); }
        uint32 byteSize() const { return capacity() * sizeof(Char); }
        uint32 capacity() const { return (mCapacity == 0 ? SMALL_BUFFER_SIZE : mCapacity); }
        uint32 hash() const { return Crc32::hash(data(), length() * sizeof(Char)); }

    private:
        uint32 mCapacity = 0;
        union {
            Char* allocated;
            Char small[SMALL_BUFFER_SIZE] = { end };
        };
    };

}

#endif //BERSERK_TSTRING_H