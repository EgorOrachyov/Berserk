/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/String/StringBuilder.hpp>
#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    StringBuilder & StringBuilder::Append(CHAR symbol) {
        mBuffer.Add(symbol);
        return *this;
    }

    StringBuilder & StringBuilder::Append(const CHAR *string) {
        using Ut = String::Utils;
        uint32 length = Ut::Length(string);
        return Append(string, length);
    }

    StringBuilder & StringBuilder::Append(const CHAR *string, uint32 length) {
        mBuffer.Add(string, length);
        return *this;
    }

    StringBuilder & StringBuilder::Append(const String &string) {
        mBuffer.Add(string.GetStr(), string.GetLength());
        return *this;
    }

    StringBuilder & StringBuilder::Append(uint32 value) {
        char buffer[64];
        snprintf(buffer, 64, "%u", value);
        return Append(buffer);
    }

    StringBuilder & StringBuilder::Append(uint64 value) {
        char buffer[128];
        snprintf(buffer, 128, "%llu", (unsigned long long int) value);
        return Append(buffer);
    }

    StringBuilder & StringBuilder::Append(int32 value) {
        char buffer[64];
        snprintf(buffer, 64, "%i", value);
        return Append(buffer);
    }

    StringBuilder & StringBuilder::Append(float value, uint32 precision) {
        char buffer[64];
        snprintf(buffer, 64, "%.*f", precision, value);
        return Append(buffer);
    }

    StringBuilder & StringBuilder::Append(double value, uint32 precision) {
        char buffer[128];
        snprintf(buffer, 128, "%.*lf", precision, value);
        return Append(buffer);
    }

    String StringBuilder::ToString() const {
        String result(mBuffer.GetData(), mBuffer.GetSize());
        return result;
    }

    // Syntax sugar section
    StringBuilder& StringBuilder::operator<<(StringBuilder::CHAR symbol) {
        return Append(symbol);
    }

    StringBuilder& StringBuilder::operator<<(const StringBuilder::CHAR* string) {
        return Append(string);
    }

    StringBuilder& StringBuilder::operator<<(const String& string) {
        return Append(string);
    }

    StringBuilder& StringBuilder::operator<<(uint32 value) {
        return Append(value);
    }

    StringBuilder& StringBuilder::operator<<(uint64 value) {
        return Append(value);
    }

    StringBuilder& StringBuilder::operator<<(int32 value) {
        return Append(value);
    }

    StringBuilder& StringBuilder::operator<<(float value) {
        return Append(value);
    }

    StringBuilder& StringBuilder::operator<<(double value) {
        return Append(value);
    }

}