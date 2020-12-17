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

    StringBuilder & StringBuilder::Add(CharType character) {
        mBuffer.Add(character);
        return *this;
    }

    StringBuilder & StringBuilder::Add(const String &string) {
        mBuffer.Add(string.GetStr(), string.GetLength());
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
        Platform::Memory::Copy(string.GetStr(), mBuffer.GetData(), mBuffer.GetSizeBytes());
        string.GetStr()[length] = String::END;
        return std::move(string);
    }

}
