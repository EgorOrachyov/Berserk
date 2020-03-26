/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CSTRINGBUILDER_H
#define BERSERK_CSTRINGBUILDER_H

#include <String/CString.h>
#include <String/CStringStatic.h>
#include <TArray.h>

namespace Berserk {

    /**
     * @brief Generic string builder
     * Allows to dynamically create string appending and
     * concatenating different types of primitives with optimized memory allocations.
     *
     * @note Contains array with char sequence. Char sequence is not null terminated.
     */
    class CStringBuilder {
    public:

        void append(const CString &string) {
            mSymbols.add(string.data(), string.length());
        }

        void append(const CStringStatic &string) {
            mSymbols.add(string.data(), string.length());
        }

        void append(const char* sequence) {
            using util = TStringUtility<char,'\0'>;
            auto length = util::length(sequence);
            append(sequence, length);
        }

        void append(const char* sequence, uint32 size) {
            mSymbols.add(sequence, size);
        }

        CString toString() {
            auto len = length();
            auto capacity = len + 1;

            CString string;
            string.ensureCapacity(capacity);
            Memory::copy(string.data(), data().data(), len);
            string.data()[len] = '\0';

            return string;
        }

        CStringStatic toStringStatic() {
            auto capacity = CStringStatic::CAPACITY;
            auto len = Math::min(length(), capacity);
            auto last = Math::min(length(), capacity-1);

            CStringStatic string;
            Memory::copy(string.data(), data().data(), len);
            string.data()[last] = '\0';

            return string;
        }

        CStringBuilder& operator+=(const char* string) {
            append(string);
            return *this;
        }

        CStringBuilder& operator+=(const CString& string) {
            append(string);
            return *this;
        }

        CStringBuilder& operator+=(const CStringStatic& string) {
            append(string);
            return *this;
        }

        uint32 length() {
            return mSymbols.size();
        }

        /** @return Char sequence of builder without null termination symbol */
        const TArray<char> &data() {
            return mSymbols;
        }

    private:
        TArray<char> mSymbols;
    };

}

#endif //BERSERK_CSTRINGBUILDER_H