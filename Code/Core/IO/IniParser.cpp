/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/IniParser.h>

namespace Berserk {

    const char IniParser::DEFAULT_SECTION_NAME[] = "General";

    IniParser::IniParser(TMap<CString,TMap<CString,IniValue>>& content, const char *source, uint32 length)
        : mContent(content),
          mSource(source),
          mLength(length) {

    }

    IniParser::EResult IniParser::nextToken() {
        while (true) {
            if (mIndex >= mLength) {
                mToken = EToken::Eof;
                return ok();
            }

            switch (mSource[mIndex]) {
                case ';':
                    mIndex += 1;
                    while (mIndex < mLength && mSource[mIndex] != '\n')
                        mIndex += 1;
                    break;

                case '\n':
                    mLine += 1;
                    mIndex += 1;
                    break;

                case '\t':
                    mIndex += 1;
                    break;

                case ' ':
                    mIndex += 1;
                    break;

                case ',':
                    mToken = EToken::Comma;
                    mIndex += 1;
                    return ok();

                case '[':
                    mToken = EToken::SquareBracketLeft;
                    mIndex += 1;
                    return ok();

                case ']':
                    mToken = EToken::SquareBracketRight;
                    mIndex += 1;
                    return ok();

                case '=':
                    mToken = EToken::Equal;
                    mIndex += 1;
                    return ok();

                case '\0':
                    mToken = EToken::Eof;
                    return ok();

                case '\"': {
                    uint32 i = mIndex + 1;
                    while (i < mLength && mSource[i] != '\"') {
                        i += 1;
                    }
                    if (i < mLength) {
                        mToken = EToken::String;
                        mString = &mSource[mIndex+1];
                        mStringLength = i - mIndex - 1;
                        mIndex = i + 1;
                        return ok();
                    }
                    return unexpectedSymbol();
                }

                case '-':
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9': {
                    uint32 i = mIndex;
                    int32 len = 0;
                    int32 number = 0;
                    int32 sign = mSource[i] == '-' ?  i++, -1 : 1;

                    while (i < mLength && isNumber(mSource[i])) {
                        number = number * 10 + (mSource[i] - '0');
                        i += 1;
                        len += 1;
                    }

                    if (i < mLength && len > 0) {
                        if (mSource[i] == '.') {
                            uint32 j = i + 1;
                            int32 fractional = 0;
                            int32 order = 1;
                            int32 fracLen = 0;

                            while (j < mLength && isNumber(mSource[j])) {
                                fractional = fractional * 10 + (mSource[j] - '0');
                                order *= 10;
                                j += 1;
                                fracLen += 1;
                            }

                            if (j < mLength && fracLen > 0) {
                                auto fn = (float)number;
                                auto ff = (float)fractional / (float)order;
                                mFloat = (float)sign * (fn + ff);
                                mToken = EToken::Float;
                                mIndex = j;

                                return ok();
                            }

                            return unexpectedSymbol();
                        }

                        mInt = sign * number;
                        mToken = EToken::Int;
                        mIndex = i;
                        return ok();
                    }

                    return unexpectedSymbol();
                }

                case 'f':
                case 't':
                case 'n':
                    if (mIndex + 4 < mLength) {
                        if (CStringUtility::compare(&mSource[mIndex], "false", 5) == 0) {
                            mToken = EToken::Bool;
                            mIndex += 5;
                            mBool = false;
                            return ok();
                        }
                    }
                    if (mIndex + 3 < mLength) {
                        if (CStringUtility::compare(&mSource[mIndex], "true", 4) == 0) {
                            mToken = EToken::Bool;
                            mIndex += 4;
                            mBool = true;
                            return ok();
                        }
                    }
                    if (mIndex + 3 < mLength) {
                        if (CStringUtility::compare(&mSource[mIndex + 1], "ull", 3) == 0) {
                            mToken = EToken::Null;
                            mIndex += 4;
                            return ok();
                        }
                    }
                default: {
                    mString = &mSource[mIndex];
                    mStringLength = 0;
                    while (mIndex < mLength && isAlphabetSymbol(mSource[mIndex])) {
                        mIndex += 1;
                        mStringLength += 1;
                    }

                    if (mStringLength == 0) {
                        return unexpectedSymbol();
                    }

                    mToken = EToken::String;
                    return ok();
                }
            }
        }
    }

    IniParser::EResult IniParser::parseArray(IniValue &value) {
        auto& array = value.getArray();
        bool mustReadAnotherValue = false;

        while (true) {
            if (nextToken() != EResult::Ok)
                return getResult();

            switch (getToken()) {
                case EToken::Null: {
                    IniValue data;
                    array.move(data);
                    break;
                }
                case EToken::Int: {
                    IniValue data;
                    data.getInt() = getInt();
                    array.move(data);
                    break;
                }
                case EToken::Float: {
                    IniValue data;
                    data.getFloat() = getFloat();
                    array.move(data);
                    break;
                }
                case EToken::String: {
                    CString string(getString(), getStringLength());
                    IniValue data;
                    data.getString() = std::move(string);
                    array.move(data);
                    break;
                }
                case EToken::Bool: {
                    IniValue data;
                    data.getBool() = getBool();
                    array.move(data);
                    break;
                }
                case EToken::SquareBracketRight: {
                    return mustReadAnotherValue? unexpectedToken() : ok();
                }
                default:
                    return unexpectedToken();
            }

            if (nextToken() != EResult::Ok)
                return getResult();

            if (getToken() == EToken::Comma) {
                mustReadAnotherValue = true;
                continue;
            }

            if (getToken() == EToken::SquareBracketRight)
                return ok();
        }
    }

    IniParser::EResult IniParser::parse() {
        auto section = &mContent[DEFAULT_SECTION_NAME];

        while (true) {
            if (nextToken() != EResult::Ok)
                return getResult();

            if (getToken() == EToken::Eof)
                return ok();

            if (getToken() == EToken::SquareBracketLeft) {

                if (nextToken() != EResult::Ok)
                    return getResult();

                if (getToken() != EToken::String)
                    return unexpectedToken();

                CString sectionName(getString(), getStringLength());

                if (nextToken() != EResult::Ok)
                    return getResult();

                if (getToken() != EToken::SquareBracketRight)
                    return unexpectedToken();

                section = &mContent[sectionName];

                continue;
            }

            if (getToken() != EToken::String)
                return unexpectedToken();

            CString key(getString(), getStringLength());

            if (nextToken() != EResult::Ok)
                return getResult();

            if (getToken() != EToken::Equal)
                return unexpectedToken();

            if (nextToken() != EResult::Ok)
                return getResult();

            switch (getToken()) {
                case EToken::Null: {
                    IniValue data;
                    section->move(key, data);
                    break;
                }
                case EToken::Int: {
                    IniValue data;
                    data.getInt() = getInt();
                    section->move(key, data);
                    break;
                }
                case EToken::Float: {
                    IniValue data;
                    data.getFloat() = getFloat();
                    section->move(key, data);
                    break;
                }
                case EToken::String: {
                    CString string(getString(), getStringLength());
                    IniValue data;
                    data.getString() = std::move(string);
                    section->move(key, data);
                    break;
                }
                case EToken::Bool: {
                    IniValue data;
                    data.getBool() = getBool();
                    section->move(key, data);
                    break;
                }
                case EToken::SquareBracketLeft: {
                    IniValue data;

                    if (parseArray(data) != EResult::Ok)
                        return getResult();

                    section->move(key, data);
                    break;
                }
                default:
                    return unexpectedToken();
            }
        }
    }

    bool IniParser::isNumber(char s) {
        return s >= '0' && s <= '9';
    }

    bool IniParser::isAlphabetSymbol(char symbol) {
        return ('a' <= symbol && symbol <= 'z') ||
               ('A' <= symbol && symbol <= 'Z') ||
               (symbol == '_') ||
               (symbol == '.') ||
               (symbol == '+') ||
               (symbol == '-');
    }

}