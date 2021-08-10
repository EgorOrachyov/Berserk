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

#ifndef BERSERK_STRING16U_H
#define BERSERK_STRING16U_H

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Templates/Contracts.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Strings/BasicString.hpp>

namespace Berserk {

    /**
     * @brief String class
     *
     * Represents dynamic utf-8 encoded string with char based storage
     * with small-string buffer optimization.
     *
     * This kind of strings are primary used for char data manipulations, ascii encoding,
     * basic num/float type conversions, concatenations, extensions extraction and etc.
     *
     * This string uses platform specific string allocator.
     */
    class String16u: public BasicString<uint16, '\0', 14> {
    public:
        /** Small buffer capacity */
        using Base = BasicString<uint16, '\0', 14>;
        /** Null terminator */
        using Base::END;
        /** Char unit */
        using Base::CharType;
        /** String utility for char unit */
        using Base::Utils;
        /** Search query result for string */
        using Base::Result;

        using Base::BasicString;

        /**
         * Converts this utf-16 encoded string to utf-8 encoded string.
         * @return True if successfully converted
         */
        bool ToUtf8(class String& out) const;
    };

}

#endif //BERSERK_STRING16U_H
