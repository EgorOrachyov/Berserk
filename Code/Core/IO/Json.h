/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_JSON_H
#define BERSERK_JSON_H

#include <TArray.h>
#include <TMap.h>
#include <Math/Math.h>
#include <Platform/IFile.h>

namespace Berserk {

    /**
     * Basic json files parser. Parses ascii encoded files as char
     * sequence. Accepts such primitives as objects, arrays, strings and null value.
     *
     * Limitations:
     *  - all the objects must be enclosed as { } values
     *  - supported only null and string as primitive types
     *  - uses list of pairs(key,value) for object representing to save some memory
     *
     * When you access values of the file via data["key"] mode it is
     * preferred to use & references in order to keep allocations count
     * as minimal as possible;
     */
    class Json {
    public:
        /**
         * Json variant value. Contains strings, arrays or json
         * objects. Values is self-contained, they could be safely modified or copied.
         */
        class Value {
        public:
            enum class Type {
                String,
                Object,
                Array,
                Null
            };

            Value() = default;
            Value(const Value& other);
            Value(Value&& other) noexcept;
            ~Value();

            Value& operator=(const Value& other);
            Value& operator=(Value&& other) noexcept;

            bool isString() const { return mType == Type::String; }
            bool isArray() const { return mType == Type::Array; }
            bool isObject() const { return mType == Type::Object; }
            bool isNull() const { return mType == Type::Null; }
            bool isNotNull() const { return mType != Type::Null; }

            void setAsNull();
            void setAsString();
            void setAsArray(IAlloc& alloc);
            void setAsObject(IAlloc& alloc);

            Type getType() const { return mType; }
            bool isPresent(const char* key) const;

            Value& operator[](const char* key);
            const Value& operator[](const char* key) const;

            Value& operator[](uint32 i);
            const Value& operator[](uint32 i) const;

            CString& getString();
            const CString& getString() const;

            TArray<Value>& getArray();
            const TArray<Value>& getArray() const;

            TArray<TPair<CString,Value>>& getObject();
            const TArray<TPair<CString,Value>>& getObject() const;

            void debug(uint32 indent = 0) const;

        private:
            Type mType = Type::Null;
            union {
                uint8 mString[sizeof(CString)];
                uint8 mArray[sizeof(TArray<Value>)];
                uint8 mObject[sizeof(TArray<TPair<CString,Value>>)] = {};
            };
        };

        Json(IAlloc& alloc = IAlloc::getSingleton());
        Json(const char* string, IAlloc& alloc = IAlloc::getSingleton());
        Json(IFile& file, IAlloc& alloc = IAlloc::getSingleton());

        Json(const Json& other) = default;
        Json(Json&& other) noexcept = default;

        Json& operator=(const Json& other) = default;
        Json& operator=(Json&& other) noexcept = default;

        Value& data() { return mRootObject; }
        const Value& data() const { return mRootObject; }

        IAlloc& getAllocator() const { return *mAlloc; }
        bool isParsed() const { return mIsParsed == Result::Ok; }

    private:
        enum class Token {
            CurlyBracketLeft = 0,
            CurlyBracketRight = 1,
            SquareBracketLeft = 2,
            SquareBracketRight = 3,
            Colon = 4,
            Comma = 5,
            String = 6,
            Null = 7,
            Eof = 8
        };

        enum class Result {
            Ok = 0,
            UnexpectedSymbol = 1,
            UnexpectedToken = 2
        };

        static const char* mTokensToString[9];
        Result getToken(const char* stream, uint32 size, Token& token, uint32& index, uint32& line, uint32& data);
        Result parseObject(const char* stream, uint32 size, Value &store, uint32 &index, uint32 &line);
        Result parseArray(const char* stream, uint32 size, Value& store, uint32& index, uint32& line);
        Result parseValue(const char* stream, uint32 size, Value& store, uint32& index, uint32& line);
        Result parse(const char* stream, uint32 size, Value& store);

    private:
        Value mRootObject;
        IAlloc* mAlloc;
        Result mIsParsed;
    };

}

#endif //BERSERK_JSON_H