/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Strings/StringBuilder.hpp>

namespace Berserk {

    void StringBuilder::EnsureToAdd(size_t symbols) {
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

    StringBuilder & StringBuilder::Add(const CharType *string, size_t length) {
        mBuffer.Add(string, length);
        return *this;
    }

    uint32 StringBuilder::GetLength() const {
        return mBuffer.GetSize();
    }

    String StringBuilder::ToString() const {
        auto length = GetLength();
        String string(length + 1);
        Memory::Copy(string.GetStr_C(), mBuffer.GetData(), mBuffer.GetSizeBytes());
        string.GetStr_C()[length] = String::END;
        return std::move(string);
    }

}
