/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TSTRINGSTATICN_H
#define BERSERK_TSTRINGSTATICN_H

#include <Math/Math.h>
#include <Platform/Memory.h>
#include <String/TStringUtility.h>

namespace Berserk {

    template <typename Char, Char end, uint32 BUFFER_CAPACITY = 64>
    class TStringStaticN {
    public:

        using Strings = TStringUtility<Char, end>;

        TStringStaticN() = default;
        ~TStringStaticN() = default;

        TStringStaticN(const TStringStaticN& other) {
            Strings::copy(mBuffer, other.mBuffer);
        }
        TStringStaticN(TStringStaticN&& other) noexcept {
            Strings::copy(mBuffer, other.mBuffer);
        }
        template <uint32 N>
        TStringStaticN(const TStringStaticN<Char,N>& other) {
            Strings::copy(mBuffer, BUFFER_CAPACITY, other.mBuffer);
        }
        TStringStaticN(const Char* buffer) {
            Strings::copy(mBuffer, BUFFER_CAPACITY, buffer);
        }

        TStringStaticN& operator=(const TStringStaticN& other) {
            Strings::copy(mBuffer, other.mBuffer);
            return *this;
        }
        TStringStaticN& operator=(const Char* other) {
            Strings::copy(mBuffer, BUFFER_CAPACITY, other);
            return *this;
        }

        TStringStaticN operator+(const TStringStaticN& other) const {
            TStringStaticN result;
            Strings::copy(result.mBuffer, mBuffer);
            Strings::concat(result.mBuffer, BUFFER_CAPACITY, other.mBuffer);
            return result;
        }
        TStringStaticN operator+(const Char* other) const {
            TStringStaticN result;
            Strings::copy(result.mBuffer, mBuffer);
            Strings::concat(result.mBuffer, BUFFER_CAPACITY, other);
            return result;
        }

        TStringStaticN& operator+=(const TStringStaticN& other) {
            Strings::concat(mBuffer, BUFFER_CAPACITY, other.mBuffer);
            return *this;
        }
        TStringStaticN& operator+=(const Char* other) {
            Strings::concat(mBuffer, BUFFER_CAPACITY, other);
            return *this;
        }

        bool operator==(const TStringStaticN& other) const {
            return Strings::compare(mBuffer, other.mBuffer) == 0;
        }
        bool operator<=(const TStringStaticN& other) const {
            return Strings::compare(mBuffer, other.mBuffer) <= 0;
        }
        bool operator>=(const TStringStaticN& other) const {
            return Strings::compare(mBuffer, other.mBuffer) >= 0;
        }
        bool operator<(const TStringStaticN& other) const {
            return Strings::compare(mBuffer, other.mBuffer) < 0;
        }
        bool operator>(const TStringStaticN& other) const {
            return Strings::compare(mBuffer, other.mBuffer) > 0;
        }

        bool replaceFirst(const TStringStaticN& substring, const TStringStaticN& replacement) {
            auto find = Strings::findFirst(mBuffer, substring.mBuffer);

            if (find != nullptr) {
                auto offset = Strings::offset(mBuffer, find);
                auto count = substring.length();

                Char destination[BUFFER_CAPACITY];
                Strings::replace(destination, BUFFER_CAPACITY, mBuffer, offset, count, replacement.mBuffer);
                Memory::copy(mBuffer, destination, BUFFER_CAPACITY * sizeof(Char));
            }

            return find != nullptr;
        }
        bool replaceLast(const TStringStaticN& substring, const TStringStaticN& replacement) {
            auto find = Strings::findLast(mBuffer, substring.mBuffer);

            if (find != nullptr) {
                auto offset = Strings::offset(mBuffer, find);
                auto count = substring.length();

                Char destination[BUFFER_CAPACITY];
                Strings::replace(destination, BUFFER_CAPACITY, mBuffer, offset, count, replacement.mBuffer);
                Memory::copy(mBuffer, destination, BUFFER_CAPACITY * sizeof(Char));
            }

            return find != nullptr;
        }

        TStringStaticN operator*(uint32 N) const {
            TStringStaticN result;
            Strings::repeat(result.mBuffer, BUFFER_CAPACITY, mBuffer, N);
            return result;
        }

        Char* findFirst(const TStringStaticN& what) {
            return Strings::findFirst(mBuffer, what.mBuffer);
        }
        const Char* findFirst(const TStringStaticN& what) const {
            return Strings::findFirst(mBuffer, what.mBuffer);
        }

        Char* findLast(const TStringStaticN& what) {
            return Strings::findLast(mBuffer, what.mBuffer);
        }
        const Char* findLast(const TStringStaticN& what) const {
            return Strings::findLast(mBuffer, what.mBuffer);
        }

        Char* data() { return mBuffer; }
        const Char* data() const { return mBuffer; }

        uint32 length() const { return Strings::length(mBuffer); }

    private:
        Char mBuffer[BUFFER_CAPACITY] = {'\0'};
    };

}

#endif //BERSERK_TSTRINGSTATICN_H