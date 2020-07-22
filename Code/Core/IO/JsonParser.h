/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_JSONPARSER_H
#define BERSERK_JSONPARSER_H

#include <IO/JsonValue.h>

namespace Berserk {

    class JsonParser {
    public:

        enum class EToken {
            CurlyBracketLeft = 0,
            CurlyBracketRight = 1,
            SquareBracketLeft = 2,
            SquareBracketRight = 3,
            Colon = 4,
            Comma = 5,
            String = 6,
            Int = 7,
            Float = 8,
            Bool = 9,
            Null = 10,
            Eof = 11
        };

        enum class EResult {
            Ok = 0,
            UnexpectedSymbol = 1,
            UnexpectedToken = 2
        };

        JsonParser(JsonValue& value, const char* source, uint32 length);
        ~JsonParser() = default;

        EResult nextToken();
        EResult parseObject(JsonValue& value);
        EResult parseArray(JsonValue& value);
        EResult parse();

        EResult getResult() const { return mResult; }
        EToken getToken() const { return mToken; }
        uint32 getIndex() const { return mIndex; }
        uint32 getLine() const { return mLine; }

        const char* getString() const { return mString; }
        uint32 getStringLength() const { return mStringLength; }
        uint32 getInt() const { return mInt; }
        float getFloat() const { return mFloat; }
        bool getBool() const { return mBool; }

        EResult unexpectedToken() { mResult = EResult::UnexpectedToken; return mResult; }
        EResult unexpectedSymbol() { mResult = EResult::UnexpectedSymbol; return mResult; }
        EResult ok() { mResult = EResult::Ok; return mResult; }

        static bool isNumber(char s);

    private:

        JsonValue& mValue;
        const char* mSource;
        uint32 mLength;
        uint32 mIndex = 0;
        uint32 mLine = 0;
        EToken mToken = EToken::Eof;
        EResult mResult = EResult::Ok;

        const char* mString = nullptr;
        uint32 mStringLength = 0;
        uint32 mInt = 0;
        float mFloat = 0.0f;
        bool mBool = false;

    };


}

#endif //BERSERK_JSONPARSER_H