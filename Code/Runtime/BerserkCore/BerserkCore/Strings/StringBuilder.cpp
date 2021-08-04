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

#include <BerserkCore/Strings/StringBuilder.hpp>

namespace Berserk {

    void StringBuilder::EnsureToAdd(uint64 symbols) {
        mBuffer.EnsureToAdd(symbols);
    }

    void StringBuilder::Clear() {
        mBuffer.Clear();
    }

    StringBuilder & StringBuilder::Add(CharType character) {
        mBuffer.Add(character);
        return *this;
    }

    StringBuilder & StringBuilder::Add(const String &string) {
        mBuffer.Add(string.GetStr_C(), string.GetLength());
        return *this;
    }

    StringBuilder & StringBuilder::Add(const StringName &string) {
        mBuffer.Add(string.GetStr_C(), string.GetLength());
        return *this;
    }

    StringBuilder & StringBuilder::Add(const CharType *string) {
        auto length = Utils::Length(string);
        return Add(string, length);
    }

    StringBuilder & StringBuilder::Add(const CharType *string, uint64 length) {
        mBuffer.Add(string, length);
        return *this;
    }

    uint32 StringBuilder::GetLength() const {
        return static_cast<uint32>(mBuffer.GetSize());
    }

    String StringBuilder::ToString() const {
        auto length = GetLength();
        String string(length + 1);
        Memory::Copy(string.GetStr_C(), mBuffer.GetData(), mBuffer.GetSizeBytes());
        string.GetStr_C()[length] = String::END;
        return std::move(string);
    }

}
