/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CSTRING_H
#define BERSERK_CSTRING_H

#include <String/TString.h>
#include <Converter.h>
#include <cstdlib>

namespace Berserk {

    template <typename T>
    class TArray;

    /** char string of dynamic size with small buffer optimization (capacity of 23 symbols + end)*/
    class CString : public TString<char,'\0', 24> {
    public:
        using TString<char,'\0', 24>::TString;
        using TString<char,'\0', 24>::operator=;
        using TString<char,'\0', 24>::operator==;
        using TString<char,'\0', 24>::operator!=;

        CString(const TString<char,'\0', 24>& string) : TString<char,'\0', 24>(string) { }

        template <uint32 N>
        static CString from(const TString<wchar,L'\0',N> &string) {
            uint32 len = string.length();
            uint32 size = (len + 1) * sizeof(wchar) / sizeof(char);

            char* tmp = (char*) StringBufferAlloc::getSingleton().allocateT<char>(size);
            std::wcstombs(tmp, string.data(), size);

            CString result = tmp;
            StringBufferAlloc::getSingleton().freeT<char>(tmp, size);

            return result;
        }

        static CString fromInt32(int32 value) {
            char buffer[32];
            Converter::int32ToString(buffer, value);
            return CString(buffer);
        }

        static CString fromInt65(int64 value) {
            char buffer[32];
            Converter::int64ToString(buffer, value);
            return CString(buffer);
        }

        static CString fromUint32(uint32 value) {
            char buffer[32];
            Converter::uint32ToString(buffer, value);
            return CString(buffer);
        }

        static CString fromUint65(uint64 value) {
            char buffer[32];
            Converter::uint64ToString(buffer, value);
            return CString(buffer);
        }

        static CString fromFloat(float value, int32 precision = 4) {
            char buffer[64];
            Converter::floatToString(buffer, value, precision);
            return CString(buffer);
        }

        static CString fromFloat(double value, int32 precision = 4) {
            char buffer[64];
            Converter::doubleToString(buffer, value, precision);
            return CString(buffer);
        }

        bool toBool() const {
            bool value;
            Converter::stringToBool(data(), value);
            return value;
        }

        int32 toInt32() const {
            int32 value;
            Converter::stringToInt32(data(), value);
            return value;
        }

        uint32 toUint32() const {
            uint32 value;
            Converter::stringToUint32(data(), value);
            return value;
        }

        float toFloat() const {
            float value;
            Converter::stringToFloat(data(), value);
            return value;
        }

        /**
         * Split provided string into parts with splitters characters
         * @param source Source string to split
         * @param splitters Splitters symbols as chars in C-style string
         * @param parts Array to store results of the split
         */
        static void split(const CString& source, const char* splitters, TArray<CString> &parts);
        
    };
}

#endif //BERSERK_CSTRING_H