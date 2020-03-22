/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_WSTRING_H
#define BERSERK_WSTRING_H

#include <String/TString.h>
#include <cstdlib>

namespace Berserk {
    /** WChar string of dynamic size with small buffer optimization (capacity of 23 symbols + end)*/
    class WString : public TString<wchar,'\0', 24/sizeof(wchar)> {
    public:
        using TString<wchar,'\0', 24/sizeof(wchar)>::TString;
        using TString<wchar,'\0', 24/sizeof(wchar)>::operator=;

        template <uint32 N>
        static WString from(const TString<char,L'\0',N> &string) {
            uint32 len = string.length();
            uint32 size = len + 1;

            wchar* tmp = (wchar*) StringBufferAlloc::getSingleton().allocateT<wchar>(size);
            std::mbstowcs(tmp, string.data(), size);

            WString result = tmp;
            StringBufferAlloc::getSingleton().freeT<wchar>(tmp, size);

            return result;
        }
    };
}

#endif //BERSERK_WSTRING_H