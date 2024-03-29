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

#include <BerserkCore/Strings/String16u.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Strings/Unicode.hpp>

bool Berserk::String16u::ToUtf8(class String &out) const {
    const CharType* buffer = GetStr_C();
    uint32 len = GetLength();

    while (len > 0) {
        uint32 parsed = len;
        uint32 encodedLen;
        Unicode::Char8u encoded[5];
        Unicode::Char32u codepoint;

        if (!Unicode::Utf16ToUtf32(buffer, parsed, codepoint))
            return false;

        if (!Unicode::Utf32toUtf8(codepoint, encoded, encodedLen))
            return false;

        out.AddChars(reinterpret_cast<const char*>(encoded), encodedLen);
        len -= parsed;
        buffer += parsed;
    }

    return true;
}
