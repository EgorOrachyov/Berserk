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
#include <String/CStringBuilder.h>

namespace Berserk {

    JsonValue::JsonValue(const JsonValue &other) {
        *this = other;
    }

    JsonValue::JsonValue(JsonValue &&other) noexcept {
        *this = std::move(other);
    }

    JsonValue::~JsonValue() {
        if (mType != EType::Null) {
            switch (mType) {
                case EType::String:
                    getString().~CString();
                    break;
                case EType::Array:
                    getArray().~TArray();
                    break;
                case EType::Object:
                    getObject().~TArray();
                    break;
                default:
                    break;
            }

            mType = EType::Null;
        }
    }

    JsonValue& JsonValue::operator=(const JsonValue &other) {
        this->~JsonValue();
        mType = other.mType;
        switch (mType) {
            case EType::String:
                new (mString) CString(other.getString());
                break;
            case EType::Array:
                new (mArray) TArray<JsonValue>(other.getArray());
                break;
            case EType::Object:
                new (mObject) TArray<TPair<CString,JsonValue>>(other.getObject());
            default:
                break;
        }
        return *this;
    }

    JsonValue& JsonValue::operator=(JsonValue &&other) noexcept {
        this->~JsonValue();
        mType = other.mType;
        switch (mType) {
            case EType::String:
                new (mString) CString(std::move(other.getString()));
                break;
            case EType::Array:
                new (mArray) TArray<JsonValue>(std::move(other.getArray()));
                break;
            case EType::Object:
                new (mObject) TArray<TPair<CString,JsonValue>>(std::move(other.getObject()));
            default:
                break;
        }
        other.mType = EType::Null;
        return *this;
    }

    void JsonValue::setAsNull() {
        this->~JsonValue();
        mType = EType::Null;
    }

    void JsonValue::setAsString() {
        this->~JsonValue();
        mType = EType::String;
        new (&getString()) CString();
    }

    void JsonValue::setAsArray(IAlloc &alloc) {
        this->~JsonValue();
        mType = EType::Array;
        new (&getArray()) TArray<JsonValue>(alloc);
    }

    void JsonValue::setAsObject(IAlloc &alloc) {
        this->~JsonValue();
        mType = EType::Object;
        new (&getObject()) TArray<TPair<CString,JsonValue>>(alloc);
    }

    JsonValue& JsonValue::operator[](const char *key) {
        BERSERK_COND_ERROR_FAIL(isObject(), "Value is not object")
        for (auto& p: getObject()) {
            if (p.first() == key) {
                return p.second();
            }
        }
        BERSERK_ERROR_FAIL("No such element")
    }

    const JsonValue& JsonValue::operator[](const char *key) const {
        BERSERK_COND_ERROR_FAIL(isObject(), "Value is not object")
        for (auto& p: getObject()) {
            if (p.first() == key) {
                return p.second();
            }
        }
        BERSERK_ERROR_FAIL("No such element")
    }

    JsonValue& JsonValue::operator[](uint32 i) {
        BERSERK_COND_ERROR_FAIL(isArray(), "Value is not array")
        return getArray()[i];
    }

    const JsonValue& JsonValue::operator[](uint32 i) const {
        BERSERK_COND_ERROR_FAIL(isArray(), "Value is not array")
        return getArray()[i];
    }

    CString& JsonValue::getString() {
        BERSERK_COND_ERROR_FAIL(isString(), "Value is not string")
        return *(CString*)mString;
    }

    const CString& JsonValue::getString() const {
        BERSERK_COND_ERROR_FAIL(isString(), "Value is not string")
        return *(CString*)mString;
    }

    TArray<JsonValue>& JsonValue::getArray() {
        BERSERK_COND_ERROR_FAIL(isArray(), "Value is not array")
        return *(TArray<JsonValue>*)mArray;
    }

    const TArray<JsonValue>& JsonValue::getArray() const {
        BERSERK_COND_ERROR_FAIL(isArray(), "Value is not array")
        return *(TArray<JsonValue>*)mArray;
    }

    TArray<TPair<CString, JsonValue>> & JsonValue::getObject() {
        BERSERK_COND_ERROR_FAIL(isObject(), "Value is not object")
        return *(TArray<TPair<CString, JsonValue>>*)mObject;
    }

    const TArray<TPair<CString, JsonValue>> & JsonValue::getObject() const {
        BERSERK_COND_ERROR_FAIL(isObject(), "Value is not object")
        return *(TArray<TPair<CString, JsonValue>>*)mObject;
    }

    bool JsonValue::accept(class JsonVisitor &visitor) {
        switch (mType) {
            case EType::String:
                return visitor.acceptString(getString());
            case EType::Array:
                return visitor.acceptArray(getArray());
            case EType::Object:
                return visitor.acceptObject(getObject());
            default:
                return false;
        }
    }

    CString JsonValue::toString() const {
        CStringBuilder builder;
        toStringBuilder(builder);
        return builder.toString();
    }

    CString JsonValue::toStringCompact() const {
        CStringBuilder builder;
        toStringBuilderCompact(builder);
        return builder.toString();
    }

    void JsonValue::toStringBuilder(class CStringBuilder &builder) const {
        builder.ensureCapacity(BUILDER_PREALLOCATE);
        writeValue(builder, 0, false);
    }

    void JsonValue::toStringBuilderCompact(class CStringBuilder &builder) const {
        builder.ensureCapacity(BUILDER_PREALLOCATE);
        writeValue(builder);
    }

    void JsonValue::writeValue(class CStringBuilder &builder) const {
        switch (mType) {
            case EType::Null: {
                builder.append("null");
                return;
            }
            case EType::String: {
                builder.append('\"');
                builder.append(getString());
                builder.append('\"');
                return;
            }
            case EType::Array: {
                builder.append('[');
                const auto& array = getArray();

                for (uint32 i = 0; i < array.size(); i++) {
                    array[i].writeValue(builder);

                    // Insert ',' if it is not last entry in the array
                    if (i + 1 < array.size()) {
                        builder.append(',');
                    }
                }

                builder.append(']');
                return;
            }
            case EType::Object: {
                builder.append('{');
                const auto& object = getObject();

                for (uint32 i = 0; i < object.size(); i++) {
                    const auto& entry = object[i];

                    builder.append('\"');
                    builder.append(entry.first());
                    builder.append('\"');
                    builder.append(':');
                    builder.append(' ');

                    entry.second().writeValue(builder);

                    // Insert ',' if it is not last entry in the object
                    if (i + 1 < object.size()) {
                        builder.append(",");
                    }
                }

                builder.append('}');
                return;
            }
            default:
                return;
        }
    }

    void JsonValue::writeValue(struct CStringBuilder &builder, uint32 indentation, bool compact) const {
        switch (mType) {
            case EType::Null: {
                if (!compact) {
                    builder.appendN(" ", indentation);
                }

                builder.append("null");
                return;
            }
            case EType::String: {
                if (!compact) {
                    builder.appendN(" ", indentation);
                }

                builder.append('\"');
                builder.append(getString());
                builder.append('\"');
                return;
            }
            case EType::Array: {
                if (!compact) {
                    builder.appendN(" ", indentation);
                }

                builder.append("[\n");
                const auto& array = getArray();

                for (uint32 i = 0; i < array.size(); i++) {
                    array[i].writeValue(builder, indentation + 1, false);

                    // Insert ',' if it is not last entry in the array
                    if (i + 1 < array.size()) {
                        builder.append(',');
                    }

                    builder.append('\n');
                }

                builder.appendN(" ", indentation);
                builder.append(']');
                return;
            }
            case EType::Object: {
                if (!compact) {
                    builder.appendN(" ", indentation);
                }

                builder.append("{\n");
                const auto& object = getObject();

                for (uint32 i = 0; i < object.size(); i++) {
                    const auto& entry = object[i];

                    builder.appendN(" ", indentation + 1);
                    builder.append('\"');
                    builder.append(entry.first());
                    builder.append('\"');
                    builder.append(':');
                    builder.append(' ');

                    entry.second().writeValue(builder, indentation + 1, true);

                    // Insert ',' if it is not last entry in the object
                    if (i + 1 < object.size()) {
                        builder.append(",");
                    }

                    builder.append('\n');
                }

                builder.appendN(" ", indentation);
                builder.append('}');
                return;
            }
            default:
                return;
        }
    }


    JsonDocument::JsonDocument(IAlloc &alloc) : mAlloc(&alloc) {

    }

    JsonDocument::JsonDocument(const char *string, IAlloc &alloc) : mAlloc(&alloc) {
        auto len = CStringUtility::length(string);
        mIsParsed = parse(string, len, mRootObject);
    }

    JsonDocument::JsonDocument(IFile &file, IAlloc &alloc) : mAlloc(&alloc) {
        BERSERK_COND_ERROR_RET(file.isOpen(), "File must be open");

        auto len = file.getSize();
        TArray<char> buffer(alloc);
        buffer.resize(len);
        file.read(buffer.data(), sizeof(char) * len);
        mIsParsed = parse(buffer.data(), len, mRootObject);
    }

    JsonDocument::EResult JsonDocument::getToken(const char* stream, uint32 size, EToken &token, uint32 &index, uint32 &line, uint32& data) {
        while (true) {
            if (index >= size) {
                token = EToken::Eof;
                return EResult::Ok;
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
                    token = EToken::CurlyBracketLeft;
                    index += 1;
                    return EResult::Ok;
                case '}':
                    token = EToken::CurlyBracketRight;
                    index += 1;
                    return EResult::Ok;
                case '[':
                    token = EToken::SquareBracketLeft;
                    index += 1;
                    return EResult::Ok;
                case ']':
                    token = EToken::SquareBracketRight;
                    index += 1;
                    return EResult::Ok;
                case ',':
                    token = EToken::Comma;
                    index += 1;
                    return EResult::Ok;
                case ':':
                    token = EToken::Colon;
                    index += 1;
                    return EResult::Ok;
                case '\0':
                    token = EToken::Eof;
                    return EResult::Ok;
                case 'n':
                    if (index + 3 < size) {
                        if (CStringUtility::compare(&stream[index + 1], "ull", 3) == 0) {
                            token = EToken::Null;
                            index += 4;
                            return EResult::Ok;
                        }
                    }
                    return EResult::UnexpectedSymbol;
                case '\"':
                    uint32 i = index + 1;
                    while (i < size && stream[i] != '\"') {
                        i += 1;
                    }
                    if (i < size) {
                        token = EToken::String;
                        data = index + 1;
                        index = i + 1;
                        return EResult::Ok;
                    }
                    return EResult::UnexpectedSymbol;
            }
        }
    }

    JsonDocument::EResult JsonDocument::parseObject(const char* stream, uint32 size, JsonValue &store, uint32 &index, uint32 &line) {
        EResult result;
        EToken token;
        uint32 data;

        store.setAsObject(*mAlloc);

        while (true) {
            result = getToken(stream, size, token, index, line, data);
            if (result != EResult::Ok) return result;

            // For empty objects
            if (token == EToken::CurlyBracketRight) return EResult::Ok;
            if (token != EToken::String) return EResult::UnexpectedToken;

            const char* key = &stream[data];
            const uint32 keyLen = index - data - 1;

            result = getToken(stream, size, token, index, line, data);
            if (result != EResult::Ok) return result;

            if (token != EToken::Colon) return EResult::UnexpectedToken;

            {
                JsonValue sub;
                result = parseValue(stream, size, sub, index, line);

                if (result != EResult::Ok) return result;

                auto& p = store.getObject().emplace();
                p.first().fromBuffer(key, keyLen);
                p.second() = std::move(sub);
            }

            result = getToken(stream, size, token, index, line, data);
            if (result != EResult::Ok) return result;

            if (token == EToken::Comma) {
                continue;
            }
            else if (token == EToken::CurlyBracketRight) {
                return EResult::Ok;
            }
            else {
                return EResult::UnexpectedToken;
            }
        }
    }

    JsonDocument::EResult JsonDocument::parseArray(const char* stream, uint32 size, JsonValue &store, uint32 &index, uint32 &line) {
        EResult result;
        EToken token;
        uint32 data;

        store.setAsArray(*mAlloc);

        while (true) {
            uint32 prevIndex = index;
            uint32 prevLine = line;

            result = getToken(stream, size, token, prevIndex, prevLine, data);
            if (result != EResult::Ok) return result;

            // For empty arrays
            if (token == EToken::SquareBracketRight) return EResult::Ok;

            {
                JsonValue sub;
                result = parseValue(stream, size, sub, index, line);

                if (result != EResult::Ok) return result;

                auto& v = store.getArray().emplace();
                v = std::move(sub);
            }

            result = getToken(stream, size, token, index, line, data);
            if (result != EResult::Ok) return result;

            if (token == EToken::Comma) {
                continue;
            }
            else if (token == EToken::SquareBracketRight) {
                return EResult::Ok;
            }
            else {
                return EResult::UnexpectedToken;
            }
        }
    }

    JsonDocument::EResult JsonDocument::parseValue(const char *stream, uint32 size, JsonValue &store, uint32 &index, uint32 &line) {
        EResult result;
        EToken token;
        uint32 data;

        result = getToken(stream, size, token, index, line, data);
        if (result != EResult::Ok) return result;

        if (token == EToken::CurlyBracketLeft) {
            return parseObject(stream, size, store, index, line);
        }
        else if (token == EToken::SquareBracketLeft) {
            return parseArray(stream, size, store, index, line);
        }
        else if (token == EToken::Null) {
            store.setAsNull();
            return EResult::Ok;
        }
        else if (token == EToken::String) {
            store.setAsString();
            store.getString().fromBuffer(&stream[data], index - data - 1);
            return EResult::Ok;
        }
        else {
            return EResult::UnexpectedToken;
        }
    }

    JsonDocument::EResult JsonDocument::parse(const char* stream, uint32 size, JsonValue& store) {
        JsonValue value;
        EToken token;
        EResult result;
        uint32 index = 0;
        uint32 line = 0;
        uint32 data;

        result = getToken(stream, size, token, index, line, data);
        if (result != EResult::Ok) return result;

        if (token != EToken::CurlyBracketLeft) return EResult::UnexpectedToken;

        result = parseObject(stream, size, value, index, line);

        if (result == EResult::Ok) {
            store = std::move(value);
        }

        return result;
    }

    bool JsonDocument::accept(class JsonVisitor &visitor) {
        return mRootObject.accept(visitor);
    }

    CString JsonDocument::toString() const {
        return mRootObject.toString();
    }

    CString JsonDocument::toStringCompact() const {
        return mRootObject.toStringCompact();
    }

    void JsonDocument::toStringBuilder(class CStringBuilder &builder) const {
        mRootObject.toStringBuilder(builder);
    }

    void JsonDocument::toStringBuilderCompact(class CStringBuilder &builder) const {
        mRootObject.toStringBuilderCompact(builder);
    }

}