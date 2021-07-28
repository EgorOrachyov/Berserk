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

#ifndef BERSERK_STRINGBUILDER_HPP
#define BERSERK_STRINGBUILDER_HPP

#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Strings/StringName.hpp>
#include <BerserkCore/Templates/Array.hpp>

namespace Berserk {

    class StringBuilder {
    public:
        using CharType = String::Char8u;
        using Utils = String::Utils;

        StringBuilder() = default;
        ~StringBuilder() = default;

        void EnsureToAdd(uint64 symbols);
        void Clear();

        StringBuilder& Add(CharType character);
        StringBuilder& Add(const String& string);
        StringBuilder& Add(const StringName& string);
        StringBuilder& Add(const CharType* string);
        StringBuilder& Add(const CharType* string, uint64 length);

        uint32 GetLength() const;

        String ToString() const;

    private:
        Array<CharType> mBuffer;

    };

}

#endif //BERSERK_STRINGBUILDER_HPP