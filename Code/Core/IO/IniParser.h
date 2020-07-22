/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_INIPARSER_H
#define BERSERK_INIPARSER_H

#include <IO/IniValue.h>

namespace Berserk {

    class IniParser {
    public:

        static const char DEFAULT_SECTION_NAME[];

        enum class EToken {
            SquareBracketLeft = 1,
            SquareBracketRight = 2,
            Comma = 3,
            Equal = 4,
            String = 5,
            Int = 6,
            Float = 7,
            Bool = 8,
            Null = 9,
            Eof = 10
        };

        enum class EResult {
            Ok = 1,
            UnexpectedSymbol = 2,
            UnexpectedToken = 3
        };

        IniParser(TMap<CString,TMap<CString,IniValue>>& content, const char* source, uint32 length);
        ~IniParser() = default;
        
        EResult nextToken();
        EResult parseArray(IniValue &array);
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
        static bool isAlphabetSymbol(char s);

    private:

        TMap<CString,TMap<CString,IniValue>> &mContent;
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

#endif //BERSERK_INIPARSER_H