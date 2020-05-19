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
#include <Containers/TArray.h>

namespace Berserk {

    /**
     * @brief Generic string builder
     *
     * Allows to dynamically create string appending and
     * concatenating different types of primitives with optimized memory allocations.
     *
     * @note Contains array with char sequence. Char sequence is not null terminated.
     */
    class CStringBuilder {
    public:

        void ensureCapacity(uint32 capacity) {
            mSymbols.ensureCapacity(capacity);
        }

        void empty() {
            mSymbols.clearNoDestructorCall();
        }

        void append(const CString &string) {
            mSymbols.add(string.data(), string.length());
        }

        void append(const CStringStatic &string) {
            mSymbols.add(string.data(), string.length());
        }

        void append(const char* sequence) {
            using util = TStringUtility<char,'\0'>;
            auto length = util::length(sequence);
            mSymbols.add(sequence, length);
        }

        void append(char symbol) {
            mSymbols.add(symbol);
        }

        void append(const char* sequence, uint32 length) {
            mSymbols.add(sequence, length);
        }

        void appendN(const char *sequence, uint32 times) {
            if (times > 0) {
                using util = TStringUtility<char,'\0'>;
                auto length = util::length(sequence);
                auto totalLength = length * times;

                if (totalLength > 0) {
                    mSymbols.ensureToAdd(totalLength);

                    for (int i = 0; i < times; i++) {
                        mSymbols.add(sequence, length);
                    }
                }
            }
        }

        CString toString() {
            auto len = length();
            auto capacity = len + 1;

            CString string;
            string.ensureCapacity(capacity);
            Memory::copy(string.data(), data(), len);
            string.data()[len] = '\0';

            return string;
        }

        CStringStatic toStringStatic() {
            auto capacity = CStringStatic::CAPACITY;
            auto len = Math::min(length(), capacity);
            auto last = Math::min(length(), capacity-1);

            CStringStatic string;
            Memory::copy(string.data(), data(), len);
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
        const char* data() {
            return mSymbols.data();
        }

    private:
        TArray<char> mSymbols;
    };

}

#endif //BERSERK_CSTRINGBUILDER_H