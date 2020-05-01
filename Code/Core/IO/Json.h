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
     * Json variant value. Contains strings, arrays or json
     * objects. Values is self-contained, they could be safely modified or copied.
     */
    class JsonValue {
    public:

        // How much symbols to preallocate inside string builder
        static const uint32 BUILDER_PREALLOCATE = 128;

        enum class EType {
            String,
            Object,
            Array,
            Null
        };

        JsonValue() = default;
        JsonValue(const JsonValue& other);
        JsonValue(JsonValue&& other) noexcept;
        ~JsonValue();

        JsonValue& operator=(const JsonValue& other);
        JsonValue& operator=(JsonValue&& other) noexcept;

        bool isString() const { return mType == EType::String; }
        bool isArray() const { return mType == EType::Array; }
        bool isObject() const { return mType == EType::Object; }
        bool isNull() const { return mType == EType::Null; }
        bool isNotNull() const { return mType != EType::Null; }

        void setAsNull();
        void setAsString();
        void setAsArray(IAlloc& alloc);
        void setAsObject(IAlloc& alloc);

        EType getType() const { return mType; }

        JsonValue& operator[](const char* key);
        JsonValue& operator[](uint32 i);
        CString& getString();
        TArray<JsonValue>& getArray();
        TArray<TPair<CString,JsonValue>>& getObject();

        const JsonValue& operator[](const char* key) const;
        const JsonValue& operator[](uint32 i) const;
        const CString& getString() const;
        const TArray<JsonValue>& getArray() const;
        const TArray<TPair<CString,JsonValue>>& getObject() const;

        /** Accept visitor to handle value content recursively */
        bool accept(class JsonVisitor &visitor);
        /** @return Serialized human readable value content */
        CString toString() const;
        /** @return Serialized compact string without extra indentations */
        CString toStringCompact() const;
        /** Write json value content into string builder */
        void toStringBuilder(class CStringBuilder& builder) const;
        /** Write compact json value content into string builder */
        void toStringBuilderCompact(class CStringBuilder& builder) const;

    private:
        friend class JsonDocument;
        void writeValue(class CStringBuilder& builder) const;
        void writeValue(struct CStringBuilder &builder, uint32 indentation, bool compact) const;

    private:
        EType mType = EType::Null;
        union {
            uint8 mString[sizeof(CString)];
            uint8 mArray[sizeof(TArray<JsonValue>)];
            uint8 mObject[sizeof(TArray<TPair<CString,JsonValue>>)] = {};
        };
    };

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
    class JsonDocument {
    public:

        JsonDocument(IAlloc& alloc = IAlloc::getSingleton());
        JsonDocument(const char* string, IAlloc& alloc = IAlloc::getSingleton());
        JsonDocument(IFile& file, IAlloc& alloc = IAlloc::getSingleton());
        JsonDocument(const JsonDocument& other) = default;
        JsonDocument(JsonDocument&& other) noexcept = default;

        JsonDocument& operator=(const JsonDocument& other) = default;
        JsonDocument& operator=(JsonDocument&& other) noexcept = default;

        JsonValue& data() { return mRootObject; }
        const JsonValue& data() const { return mRootObject; }

        IAlloc& getAllocator() const { return *mAlloc; }
        bool isParsed() const { return mIsParsed == EResult::Ok; }
        bool accept(class JsonVisitor &visitor);

        CString toString() const;
        CString toStringCompact() const;
        void toStringBuilder(class CStringBuilder& builder) const;
        void toStringBuilderCompact(class CStringBuilder& builder) const;

    private:
        enum class EToken {
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

        enum class EResult {
            Ok = 0,
            UnexpectedSymbol = 1,
            UnexpectedToken = 2
        };

        EResult getToken(const char* stream, uint32 size, EToken& token, uint32& index, uint32& line, uint32& data);
        EResult parseObject(const char* stream, uint32 size, JsonValue &store, uint32 &index, uint32 &line);
        EResult parseArray(const char* stream, uint32 size, JsonValue& store, uint32& index, uint32& line);
        EResult parseValue(const char* stream, uint32 size, JsonValue& store, uint32& index, uint32& line);
        EResult parse(const char* stream, uint32 size, JsonValue& store);

    private:
        JsonValue mRootObject;
        IAlloc* mAlloc;
        EResult mIsParsed = EResult::Ok;
    };

    /**
     * Generic visitor class for safe Json document traversal.
     * Override this methods in order to access json document content.
     */
    class JsonVisitor {
    public:

        /** @return True if data successfully accepted */
        virtual bool acceptString(CString& value) { return false; }

        /** @return True if data successfully accepted */
        virtual bool acceptArray(TArray<JsonValue>& body) { return false; }

        /** @return True if data successfully accepted */
        virtual bool acceptObject(TArray<TPair<CString,JsonValue>>& body) { return false; }

    };

}

#endif //BERSERK_JSON_H