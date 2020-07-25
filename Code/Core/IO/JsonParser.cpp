/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/JsonParser.h>
#include <Converter.h>

namespace Berserk {

    JsonParser::JsonParser(JsonValue &value, const char *source, uint32 length)
        : mValue(value), mSource(source), mLength(length) {

    }

    JsonParser::EResult JsonParser::nextToken() {
        while (true) {
            if (mIndex >= mLength) {
                mToken = EToken::Eof;
                return ok();
            }

            switch (mSource[mIndex]) {
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

                case '{':
                    mToken = EToken::CurlyBracketLeft;
                    mIndex += 1;
                    return ok();

                case '}':
                    mToken = EToken::CurlyBracketRight;
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

                case ',':
                    mToken = EToken::Comma;
                    mIndex += 1;
                    return ok();

                case ':':
                    mToken = EToken::Colon;
                    mIndex += 1;
                    return ok();

                case '\0':
                    mToken = EToken::Eof;
                    return ok();

                case 'n': {
                    if (mIndex + 3 < mLength) {
                        if (CStringUtility::compare(&mSource[mIndex + 1], "ull", 3) == 0) {
                            mToken = EToken::Null;
                            mIndex += 4;
                            return ok();
                        }
                    }
                    return unexpectedSymbol();
                }

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

                case 'f': {
                    if (mIndex + 4 < mLength) {
                        if (CStringUtility::compare(&mSource[mIndex], "false", 5) == 0) {
                            mToken = EToken::Bool;
                            mIndex += 5;
                            mBool = false;
                            return ok();
                        }
                    }

                    return unexpectedSymbol();
                }

                case 't': {
                    if (mIndex + 3 < mLength) {
                        if (CStringUtility::compare(&mSource[mIndex], "true", 4) == 0) {
                            mToken = EToken::Bool;
                            mIndex += 4;
                            mBool = true;
                            return ok();
                        }
                    }

                    return unexpectedSymbol();
                }
            }
        }    
    }

    JsonParser::EResult JsonParser::parseArray(JsonValue &value) {
        auto& arr = value.getArray();
        bool mustReadAnotherValue = false;

        while (true) {
            if (nextToken() != EResult::Ok)
                return getResult();

            switch (getToken()) {
                case EToken::Null: {
                    JsonValue dataAsJson;
                    arr.move(dataAsJson);
                    break;
                }
                case EToken::String: {
                    CString data(getString(), getStringLength());
                    JsonValue dataAsJson;
                    dataAsJson.getString() = std::move(data);
                    arr.move(dataAsJson);
                    break;
                }
                case EToken::Int: {
                    JsonValue dataAsJson;
                    dataAsJson.getInt() = getInt();
                    arr.move(dataAsJson);
                    break;
                }
                case EToken::Float: {
                    JsonValue dataAsJson;
                    dataAsJson.getFloat() = getFloat();
                    arr.move(dataAsJson);
                    break;
                }
                case EToken::Bool: {
                    JsonValue dataAsJson;
                    dataAsJson.getBool() = getBool();
                    arr.move(dataAsJson);
                    break;
                }
                case EToken::CurlyBracketLeft: {
                    JsonValue subObject;

                    if (parseObject(subObject) != EResult::Ok)
                        return getResult();

                    arr.move(subObject);
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

            return unexpectedToken();
        }
    }

    JsonParser::EResult JsonParser::parseObject(JsonValue &value) {
        auto& obj = value.getMap();

        while (true) {
            if (nextToken() != EResult::Ok)
                return getResult();

            if (getToken() != EToken::String && getToken() != EToken::CurlyBracketRight)
                return unexpectedToken();

            if (getToken() == EToken::CurlyBracketRight)
                return ok();

            CString keyData(getString(), getStringLength());
            JsonValue key; key.getString() = std::move(keyData);

            if (nextToken() != EResult::Ok)
                return getResult();

            if (getToken() != EToken::Colon)
                mResult = EResult::UnexpectedToken;

            if (nextToken() != EResult::Ok)
                return getResult();

            switch (getToken()) {
                case EToken::Null: {
                    JsonValue dataAsJson;
                    obj.move(key, dataAsJson);
                    break;
                }
                case EToken::String: {
                    CString data(getString(), getStringLength());
                    JsonValue dataAsJson;
                    dataAsJson.getString() = std::move(data);
                    obj.move(key, dataAsJson);
                    break;
                }
                case EToken::Int: {
                    JsonValue dataAsJson;
                    dataAsJson.getInt() = getInt();
                    obj.move(key, dataAsJson);
                    break;
                }
                case EToken::Float: {
                    JsonValue dataAsJson;
                    dataAsJson.getFloat() = getFloat();
                    obj.move(key, dataAsJson);
                    break;
                }
                case EToken::Bool: {
                    JsonValue dataAsJson;
                    dataAsJson.getBool() = getBool();
                    obj.move(key, dataAsJson);
                    break;
                }
                case EToken::CurlyBracketLeft: {
                    JsonValue subObject;

                    if (parseObject(subObject) != EResult::Ok)
                        return getResult();

                    obj.move(key, subObject);
                    break;
                }
                case EToken::SquareBracketLeft: {
                    JsonValue subArray;

                    if (parseArray(subArray) != EResult::Ok)
                        return getResult();

                    obj.move(key, subArray);
                    break;
                }
                default:
                    return unexpectedToken();
            }

            if (nextToken() != EResult::Ok)
                return getResult();

            if (getToken() == EToken::Comma)
                continue;

            if (getToken() == EToken::CurlyBracketRight)
                return EResult::Ok;

            return unexpectedToken();
        }
    }

    JsonParser::EResult JsonParser::parse() {
        if (nextToken() != EResult::Ok)
            return getResult();

        if (getToken() != EToken::CurlyBracketLeft)
            return unexpectedToken();

        JsonValue value;

        if (parseObject(value) != EResult::Ok)
            return getResult();

        mValue = std::move(value);

        return EResult::Ok;
    }

    bool JsonParser::isNumber(char s) {
        return s >= '0' && s <= '9';
    }

}