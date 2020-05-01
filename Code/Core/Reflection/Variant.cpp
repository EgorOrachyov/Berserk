/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Reflection/Variant.h>
#include <String/CStringBuilder.h>
#include <Crc32.h>

namespace Berserk {

    Variant::Variant(Variant::Bool value) {
        mType = EVariantType::Bool;
        *getBoolRaw() = value;
    }

    Variant::Variant(Variant::Int value) {
        mType = EVariantType::Int;
        *getIntRaw() = value;
    }

    Variant::Variant(Variant::Float value) {
        mType = EVariantType::Float;
        *getFloatRaw() = value;
    }

    Variant::Variant(const char *string) {
        mType = EVariantType::String;
        new (getData()) String(string);
    }

    Variant::Variant(const Variant::String &string) {
        mType = EVariantType::String;
        new (getData()) String(string);
    }

    Variant::Variant(const CStringStatic &string) {
        mType = EVariantType::String;
        new (getData()) String(string.data());
    }

    Variant::Variant(const Variant::Array &array) {
        mType = EVariantType::Array;
        new (getData()) Array(array);
    }

    Variant::Variant(const Variant::Map &map) {
        mType = EVariantType::Map;
        getMapRaw() = (Map*) Memory::allocate(sizeof(Map));
        new (getMapRaw()) Map(map);
    }

    Variant::~Variant() {
        switch (mType) {
            case EVariantType::Null:
                break;
            case EVariantType::Bool:
                break;
            case EVariantType::Int:
                break;
            case EVariantType::Float:
                break;
            case EVariantType::String:
                getStringRaw()->~String();
                break;
            case EVariantType::Array:
                getArrayRaw()->~Array();
                break;
            case EVariantType::Map:
                getMapRaw()->~Map();
                Memory::free(getMapRaw());
                break;
            default:
                break;
        }

        mType = EVariantType::Null;
    }

    void Variant::asNull() {
        if (mType != EVariantType::Null) {
            mType = EVariantType::Null;
        }
    }

    void Variant::asBool() {
        if (mType != EVariantType::Bool) {
            this->~Variant();
            *getBoolRaw() = false;
            mType = EVariantType::Bool;
        }
    }

    void Variant::asInt() {
        if (mType != EVariantType::Int) {
            this->~Variant();
            *getIntRaw() = 0;
            mType = EVariantType::Int;
        }
    }

    void Variant::asFloat() {
        if (mType != EVariantType::Float) {
            this->~Variant();
            *getFloatRaw() = 0.0f;
            mType = EVariantType::Float;
        }
    }

    void Variant::asString() {
        if (mType != EVariantType::String) {
            this->~Variant();
            new (getData()) CString();
            mType = EVariantType::String;
        }
    }

    void Variant::asArray() {
        if (mType != EVariantType::Array) {
            this->~Variant();
            new (getData()) Array();
            mType = EVariantType::Array;
        }
    }

    void Variant::asMap() {
        if (mType != EVariantType::Map) {
            this->~Variant();
            getMapRaw() = (Map*) Memory::allocate(sizeof(Map));
            new (getMapRaw()) Map();
            mType = EVariantType::Map;
        }
    }

    uint32 Variant::hash() const {
        switch (mType) {
            case EVariantType::Null:
                return 0;
            case EVariantType::Bool:
                return (*getBoolRaw() ? 1: 0);
            case EVariantType::Int:
                return Crc32::hash(getIntRaw(), sizeof(Int));
            case EVariantType::Float:
                return Crc32::hash(getFloatRaw(), sizeof(Float));
            case EVariantType::String:
                return getStringRaw()->hash();
            case EVariantType::Array:
                return 0; // may be add later
            case EVariantType::Map:
                return 0; // may be add later
            default:
                return 0;
        }
    }

    CString Variant::toString() const {
        switch (mType) {
            case EVariantType::Null:
                return "Null";
            case EVariantType::Bool:
                return (*getBoolRaw()? "true": "false");
            case EVariantType::Int:
                return CString::fromInt32(*getIntRaw());
            case EVariantType::Float:
                return CString::fromFloat(*getFloatRaw());
            case EVariantType::String: {
                String& data = *getStringRaw();
                String string;
                string.ensureCapacity(data.length() + 3);
                string += "\"";
                string += data;
                string += "\"";
            }
            case EVariantType::Array: {
                CStringBuilder builder;
                builder.ensureCapacity(BUILDER_PREALLOCATE);
                this->toStringBuilder(builder);

                return builder.toString();
            }
            case EVariantType::Map: {
                CStringBuilder builder;
                builder.ensureCapacity(BUILDER_PREALLOCATE);
                this->toStringBuilder(builder);

                return builder.toString();
            }
            default:
                return "Undefined";
        }
    }

    void Variant::toStringBuilder(class CStringBuilder &builder) const {
        switch (mType) {
            case EVariantType::Null:
                builder.append("null");
                return;
            case EVariantType::Bool:
                builder.append((*getBoolRaw()? "true": "false"));
                return;
            case EVariantType::Int: {
                char buffer[32];
                Converter::int32ToString(buffer, *getIntRaw());
                builder.append(buffer);
                return;
            }
            case EVariantType::Float: {
                char buffer[32];
                Converter::floatToString(buffer, *getFloatRaw());
                builder.append(buffer);
                return;
            }
            case EVariantType::String: {
                builder.append('\"');
                builder.append(*getStringRaw());
                builder.append('\"');
                return;
            }
            case EVariantType::Array: {
                builder.append('[');
                auto& array = *getArrayRaw();

                for (uint32 i = 0; i < array.size(); i++) {
                    array[i].toStringBuilder(builder);

                    if (i + 1 < array.size()) {
                        builder.append(',');
                    }
                }

                builder.append(']');
                return;
            }
            case EVariantType::Map: {
                builder.append('{');
                auto& map = *getMapRaw();

                uint32 i = 0;
                uint32 size = map.size();

                for (const auto& pair: map) {
                    pair.first().toStringBuilder(builder);
                    builder.append(':');
                    pair.second().toStringBuilder(builder);

                    if (i + 1 < size) {
                        builder.append(',');
                    }

                    i += 1;
                }

                builder.append('}');
                return;
            }
            default:
                return;
        }
    }

    bool Variant::operator==(const Variant &variant) const {
        if (mType != variant.mType) return false;

        switch (mType) {
            case EVariantType::Null:
                return true;
            case EVariantType::Bool:
                return *getBoolRaw() == *getBoolRaw();
            case EVariantType::Int:
                return *getIntRaw() == *variant.getIntRaw();
            case EVariantType::Float:
                return *getFloatRaw() == *variant.getFloatRaw();
            case EVariantType::String:
                return *getStringRaw() == *variant.getStringRaw();
            case EVariantType::Array:
                return *getArrayRaw() == *variant.getArrayRaw();
            case EVariantType::Map:
                return *getMapRaw() == *variant.getMapRaw();
            default:
                return false;
        }
    }

    bool Variant::operator!=(const Variant &variant) const {
        if (mType != variant.mType) return true;

        switch (mType) {
            case EVariantType::Null:
                return false;
            case EVariantType::Bool:
                return *getBoolRaw() != *variant.getBoolRaw();
            case EVariantType::Int:
                return *getIntRaw() != *variant.getIntRaw();
            case EVariantType::Float:
                return *getFloatRaw() != *variant.getFloatRaw();
            case EVariantType::String:
                return *getStringRaw() != *variant.getStringRaw();
            case EVariantType::Array:
                return *getArrayRaw() != *variant.getArrayRaw();
            case EVariantType::Map:
                return *getMapRaw() != *variant.getMapRaw();
            default:
                return false;
        }
    }

    Variant::Bool& Variant::getBool() {
        asBool();
        return *getBoolRaw();
    }

    Variant::Int& Variant::getInt() {
        asInt();
        return *getIntRaw();
    }

    Variant::Float& Variant::getFloat() {
        asFloat();
        return *getFloatRaw();
    }

    Variant::String& Variant::getString() {
        asString();
        return *getStringRaw();
    }

    Variant::Array& Variant::getArray() {
        asArray();
        return *getArrayRaw();
    }

    Variant::Map& Variant::getMap() {
        asMap();
        return *getMapRaw();
    }

    Variant::Bool* Variant::getBoolRaw() const {
        return (Bool*) &mData;
    }

    Variant::Int* Variant::getIntRaw() const {
        return (Int*) &mData;
    }

    Variant::Float* Variant::getFloatRaw() const {
        return (Float*) &mData;
    }

    Variant::String* Variant::getStringRaw() const {
        return (String*) &mData;
    }

    Variant::Array* Variant::getArrayRaw() const {
        return (Array*) &mData;
    }

    Variant::Map* &Variant::getMapRaw() const {
        return (Map* &)(mData);
    }

    void* Variant::getData() const {
        return (void*) mData;
    }

    const char* Variant::getVariantTypeString(EVariantType type) {
        switch (type) {
            case EVariantType::Null:
                return "Null";
            case EVariantType::Int:
                return "Int";
            case EVariantType::Float:
                return "Float";
            case EVariantType::String:
                return "String";
            case EVariantType::Array:
                return "Array";
            case EVariantType::Map:
                return "Map";
            default:
                return "Undefined";
        }
    }

}