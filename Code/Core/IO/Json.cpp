/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/Json.h>
#include <ErrorMacro.h>
#include <String/TStringUtility.h>
#include <String/CStringStatic.h>

namespace Berserk {

    Json::Value::Value(const Berserk::Json::Value &other) {
        *this = other;
    }

    Json::Value::Value(Json::Value &&other) noexcept {
        *this = std::move(other);
    }

    Json::Value::~Value() {
        if (mType != Type::Null) {
            switch (mType) {
                case Type::String:
                    getString().~CString();
                    break;
                case Type::Array:
                    getArray().~TArray();
                    break;
                case Type::Object:
                    getObject().~TArray();
                    break;
                default:
                    break;
            }

            mType = Type::Null;
        }
    }

    Json::Value& Json::Value::operator=(const Berserk::Json::Value &other) {
        this->~Value();
        mType = other.mType;
        switch (mType) {
            case Type::String:
                new (mString) CString(other.getString());
                break;
            case Type::Array:
                new (mArray) TArray<Value>(other.getArray());
                break;
            case Type::Object:
                new (mObject) TArray<TPair<CString,Value>>(other.getObject());
            default:
                break;
        }
        return *this;
    }

    Json::Value& Json::Value::operator=(Json::Value &&other) noexcept {
        this->~Value();
        mType = other.mType;
        switch (mType) {
            case Type::String:
                new (mString) CString(std::move(other.getString()));
                break;
            case Type::Array:
                new (mArray) TArray<Value>(std::move(other.getArray()));
                break;
            case Type::Object:
                new (mObject) TArray<TPair<CString,Value>>(std::move(other.getObject()));
            default:
                break;
        }
        other.mType = Type::Null;
        return *this;
    }

    void Json::Value::setAsNull() {
        this->~Value();
        mType = Type::Null;
    }

    void Json::Value::setAsString() {
        this->~Value();
        mType = Type::String;
        new (&getString()) CString();
    }

    void Json::Value::setAsArray(IAlloc &alloc) {
        this->~Value();
        mType = Type::Array;
        new (&getArray()) TArray<Value>(alloc);
    }

    void Json::Value::setAsObject(IAlloc &alloc) {
        this->~Value();
        mType = Type::Object;
        new (&getObject()) TArray<TPair<CString,Value>>(alloc);
    }

    bool Json::Value::isPresent(const char *key) const {
        BERSERK_COND_ERROR_FAIL(isObject(), "Value is not object")
        for (auto& p: getObject()) {
            if (p.first() == key) {
                return true;
            }
        }
        return false;
    }

    Json::Value& Json::Value::operator[](const char *key) {
        BERSERK_COND_ERROR_FAIL(isObject(), "Value is not object")
        for (auto& p: getObject()) {
            if (p.first() == key) {
                return p.second();
            }
        }
        BERSERK_ERROR_FAIL("No such element")
    }

    const Json::Value& Json::Value::operator[](const char *key) const {
        BERSERK_COND_ERROR_FAIL(isObject(), "Value is not object")
        for (auto& p: getObject()) {
            if (p.first() == key) {
                return p.second();
            }
        }
        BERSERK_ERROR_FAIL("No such element")
    }

    Json::Value& Json::Value::operator[](uint32 i) {
        BERSERK_COND_ERROR_FAIL(isArray(), "Value is not array")
        return getArray()[i];
    }

    const Json::Value& Json::Value::operator[](uint32 i) const {
        BERSERK_COND_ERROR_FAIL(isArray(), "Value is not array")
        return getArray()[i];
    }

    CString& Json::Value::getString() {
        BERSERK_COND_ERROR_FAIL(isString(), "Value is not string")
        return *(CString*)mString;
    }

    const CString& Json::Value::getString() const {
        BERSERK_COND_ERROR_FAIL(isString(), "Value is not string")
        return *(CString*)mString;
    }

    TArray<Json::Value>& Json::Value::getArray() {
        BERSERK_COND_ERROR_FAIL(isArray(), "Value is not array")
        return *(TArray<Json::Value>*)mArray;
    }

    const TArray<Json::Value>& Json::Value::getArray() const {
        BERSERK_COND_ERROR_FAIL(isArray(), "Value is not array")
        return *(TArray<Json::Value>*)mArray;
    }

    TArray<TPair<CString, Json::Value>> & Json::Value::getObject() {
        BERSERK_COND_ERROR_FAIL(isObject(), "Value is not object")
        return *(TArray<TPair<CString, Json::Value>>*)mObject;
    }

    const TArray<TPair<CString, Json::Value>> & Json::Value::getObject() const {
        BERSERK_COND_ERROR_FAIL(isObject(), "Value is not object")
        return *(TArray<TPair<CString, Json::Value>>*)mObject;
    }

    void Json::Value::debug(Berserk::uint32 indent) const {
        switch (mType) {
            case Type::Null:
                printf("null");
                return;
            case Type::String:
                printf("\"%s\"", getString().data());
                return;
            case Type::Array:
                printf("[\n");
                for (uint32 i = 0; i < getArray().size(); i++) {
                    if (i > 0) {
                        printf(",\n");
                    }

                    printf("%s", (CStringStatic{" "} * (indent + 1)).data());
                    getArray()[i].debug(indent + 1);
                }
                printf("\n");
                printf("%s]", (CStringStatic{" "} * (indent)).data());
                return;
            case Type::Object:
                printf("{\n");
                for (uint32 i = 0; i < getObject().size(); i++) {
                    if (i > 0) {
                        printf(",\n");
                    }

                    printf("%s", (CStringStatic{" "} * (indent + 1)).data());
                    printf("%s: ", getObject()[i].first().data());
                    getObject()[i].second().debug(indent + 1);
                }
                printf("\n");
                printf("%s}", (CStringStatic{" "} * (indent)).data());
                return;
        }
    }

    Json::Json(Berserk::IAlloc &alloc) : mAlloc(&alloc) {

    }

    Json::Json(const char *string, IAlloc &alloc) : mAlloc(&alloc) {
        auto len = CStringUtility::length(string);
        mIsParsed = parse(string, len, mRootObject);
    }

    Json::Json(IFile &file, IAlloc &alloc) : mAlloc(&alloc) {
        auto len = file.getSize();
        TArray<char> buffer(alloc);
        buffer.ensureCapacity(len);
        file.read(buffer.data(), sizeof(char) * len);
        mIsParsed = parse(buffer.data(), len, mRootObject);
    }

    const char* Json::mTokensToString[9] = {
            "{", "}", "[", "]", ":", ",", "string", "null", "eof"
    };

    Json::Result Json::getToken(const char* stream, uint32 size, Token &token, uint32 &index, uint32 &line, uint32& data) {
        while (true) {
            if (index >= size) {
                token = Token::Eof;
                return Result::Ok;
            }

            data = index;

            switch (stream[index]) {
                case '\n':
                    line += 1;
                    index += 1;
                    break;
                case '\t':
                    index += 1;
                    break;
                case ' ':
                    index += 1;
                    break;
                case '{':
                    token = Token::CurlyBracketLeft;
                    index += 1;
                    return Result::Ok;
                case '}':
                    token = Token::CurlyBracketRight;
                    index += 1;
                    return Result::Ok;
                case '[':
                    token = Token::SquareBracketLeft;
                    index += 1;
                    return Result::Ok;
                case ']':
                    token = Token::SquareBracketRight;
                    index += 1;
                    return Result::Ok;
                case ',':
                    token = Token::Comma;
                    index += 1;
                    return Result::Ok;
                case ':':
                    token = Token::Colon;
                    index += 1;
                    return Result::Ok;
                case '\0':
                    token = Token::Eof;
                    return Result::Ok;
                case 'n':
                    if (index + 3 < size) {
                        if (CStringUtility::compare(&stream[index + 1], "ull", 3) == 0) {
                            token = Token::Null;
                            index += 4;
                            return Result::Ok;
                        }
                    }
                    return Result::UnexpectedSymbol;
                case '\"':
                    uint32 i = index + 1;
                    while (i < size && stream[i] != '\"') {
                        i += 1;
                    }
                    if (i < size) {
                        token = Token::String;
                        data = index + 1;
                        index = i + 1;
                        return Result::Ok;
                    }
                    return Result::UnexpectedSymbol;
            }
        }
    }

    Json::Result Json::parseObject(const char* stream, uint32 size, Value &store, uint32 &index, uint32 &line) {
        Result result;
        Token token;
        uint32 data;

        store.setAsObject(*mAlloc);

        while (true) {
            result = getToken(stream, size, token, index, line, data);
            if (result != Result::Ok) return result;

            // For empty objects
            if (token == Token::CurlyBracketRight) return Result::Ok;
            if (token != Token::String) return Result::UnexpectedToken;

            const char* key = &stream[data];
            const uint32 keyLen = index - data - 1;

            result = getToken(stream, size, token, index, line, data);
            if (result != Result::Ok) return result;

            if (token != Token::Colon) return Result::UnexpectedToken;

            {
                Value sub;
                result = parseValue(stream, size, sub, index, line);

                if (result != Result::Ok) return result;

                auto& p = store.getObject().emplace();
                p.first().fromBuffer(key, keyLen);
                p.second() = std::move(sub);
            }

            result = getToken(stream, size, token, index, line, data);
            if (result != Result::Ok) return result;

            if (token == Token::Comma) {
                continue;
            }
            else if (token == Token::CurlyBracketRight) {
                return Result::Ok;
            }
            else {
                return Result::UnexpectedToken;
            }
        }
    }

    Json::Result Json::parseArray(const char* stream, uint32 size, Json::Value &store, uint32 &index, uint32 &line) {
        Result result;
        Token token;
        uint32 data;

        store.setAsArray(*mAlloc);

        while (true) {
            uint32 prevIndex = index;
            uint32 prevLine = line;

            result = getToken(stream, size, token, prevIndex, prevLine, data);
            if (result != Result::Ok) return result;

            // For empty arrays
            if (token == Token::SquareBracketRight) return Result::Ok;

            {
                Value sub;
                result = parseValue(stream, size, sub, index, line);

                if (result != Result::Ok) return result;

                auto& v = store.getArray().emplace();
                v = std::move(sub);
            }

            result = getToken(stream, size, token, index, line, data);
            if (result != Result::Ok) return result;

            if (token == Token::Comma) {
                continue;
            }
            else if (token == Token::SquareBracketRight) {
                return Result::Ok;
            }
            else {
                return Result::UnexpectedToken;
            }
        }
    }

    Json::Result Json::parseValue(const char *stream, uint32 size, Json::Value &store, uint32 &index, uint32 &line) {
        Result result;
        Token token;
        uint32 data;

        result = getToken(stream, size, token, index, line, data);
        if (result != Result::Ok) return result;

        if (token == Token::CurlyBracketLeft) {
            return parseObject(stream, size, store, index, line);
        }
        else if (token == Token::SquareBracketLeft) {
            return parseArray(stream, size, store, index, line);
        }
        else if (token == Token::Null) {
            store.setAsNull();
            return Result::Ok;
        }
        else if (token == Token::String) {
            store.setAsString();
            store.getString().fromBuffer(&stream[data], index - data - 1);
            return Result::Ok;
        }
        else {
            return Result::UnexpectedToken;
        }
    }

    Json::Result Json::parse(const char* stream, uint32 size, Value& store) {
        Value value;
        Token token;
        Result result;
        uint32 index = 0;
        uint32 line = 0;
        uint32 data;

        result = getToken(stream, size, token, index, line, data);
        if (result != Result::Ok) return result;

        if (token != Token::CurlyBracketLeft) return Result::UnexpectedToken;

        result = parseObject(stream, size, value, index, line);

        if (result == Result::Ok) {
            store = std::move(value);
        }

        return result;
    }

}