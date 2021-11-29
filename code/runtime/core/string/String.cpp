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

#include <core/string/String.hpp>

#include <cstdlib>

BRK_NS_BEGIN

float StringUtils::ToFloat(const String &str) {
    return std::strtof(str.c_str(), nullptr);
}

double StringUtils::ToDouble(const String &str) {
    return std::strtod(str.c_str(), nullptr);
}

int32 StringUtils::ToInt32(const String &str) {
    const int32 BASE = 10;
    return static_cast<int32>(std::strtoll(str.c_str(), nullptr, BASE));
}

int64 StringUtils::ToInt64(const String &str) {
    const int32 BASE = 10;
    return static_cast<int64>(std::strtoll(str.c_str(), nullptr, BASE));
}

uint32 StringUtils::ToUint32(const String &str) {
    const int32 BASE = 10;
    return static_cast<uint32>(std::strtoull(str.c_str(), nullptr, BASE));
}

uint64 StringUtils::ToUint64(const String &str) {
    const int32 BASE = 10;
    return static_cast<uint64>(std::strtoull(str.c_str(), nullptr, BASE));
}

String StringUtils::From(int value) {
    CharType buffer[64];
    snprintf(buffer, 64, "%i", value);
    return buffer;
}

String StringUtils::From(unsigned int value) {
    CharType buffer[128];
    snprintf(buffer, 128, "%u", value);
    return buffer;
}

String StringUtils::From(long value) {
    CharType buffer[64];
    snprintf(buffer, 64, "%li", value);
    return buffer;
}

String StringUtils::From(unsigned long value) {
    CharType buffer[128];
    snprintf(buffer, 128, "%lu", value);
    return buffer;
}

String StringUtils::Fromf(float value, uint32 precision) {
    CharType buffer[64];
    snprintf(buffer, 64, "%.*f", precision, value);
    return buffer;
}

String StringUtils::Fromd(double value, uint32 precision) {
    CharType buffer[128];
    snprintf(buffer, 128, "%.*lf", precision, value);
    return buffer;
}

String StringUtils::Fromi32(int32 value) {
    CharType buffer[64];
    snprintf(buffer, 64, "%i", static_cast<int>(value));
    return buffer;
}

String StringUtils::Fromi64(int64 value) {
    CharType buffer[128];
    snprintf(buffer, 128, "%lli", static_cast<long long int>(value));
    return buffer;
}

String StringUtils::Fromu32(uint32 value) {
    CharType buffer[64];
    snprintf(buffer, 64, "%u", static_cast<unsigned int>(value));
    return buffer;
}

String StringUtils::Fromu64(uint64 value) {
    CharType buffer[128];
    snprintf(buffer, 128, "%llu", static_cast<unsigned long long int>(value));
    return buffer;
}

String StringUtils::Fromb(bool value) {
    return value ? BRK_TEXT("True") : BRK_TEXT("False");
}

String StringUtils::Fromp(const void *value) {
    CharType buffer[128];
    snprintf(buffer, 128, "%p", value);
    return buffer;
}

BRK_NS_END