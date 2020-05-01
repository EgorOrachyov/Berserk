/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VARIANT_H
#define BERSERK_VARIANT_H

#include <Typedefs.h>
#include <TArray.h>
#include <TMap.h>
#include <String/CString.h>

namespace Berserk {

    enum class EVariantType {
        Null,
        Bool,
        Int,
        Float,
        String,
        Array,
        Map
    };

    class Variant {
    public:

        // How much symbols to preallocate inside string builder
        static const uint32 BUILDER_PREALLOCATE = 128;

        using Bool = bool;
        using Int = int32;
        using Float = float;
        using String = CString;
        using Array = TArray<Variant>;
        using Map = TMap<Variant,Variant>;

        Variant() = default;

        Variant(Bool value);
        Variant(Int value);
        Variant(Float value);
        Variant(const char *string);
        Variant(const String &string);
        Variant(const CStringStatic &string);
        Variant(const Array &array);
        Variant(const Map &map);

        ~Variant();

        void asNull();
        void asBool();
        void asInt();
        void asFloat();
        void asString();
        void asArray();
        void asMap();

        Bool&   getBool();
        Int&    getInt();
        Float&  getFloat();
        String& getString();
        Array&  getArray();
        Map&    getMap();

        bool isNull() const { return mType == EVariantType::Null; }
        bool isBool() const { return mType == EVariantType::Bool; }
        bool isInt() const { return mType == EVariantType::Int; }
        bool isFloat() const { return mType == EVariantType::Float; }
        bool isString() const { return mType == EVariantType::String; }
        bool isArray() const { return mType == EVariantType::Array; }
        bool isMap() const { return mType == EVariantType::Map; }
        bool isNotNull() const { return mType != EVariantType::Null; }

        uint32 hash() const;
        EVariantType getType() const { return mType; }
        CString toString() const;
        void toStringBuilder(class CStringBuilder& builder) const;

        bool operator==(const Variant& variant) const;
        bool operator!=(const Variant& variant) const;

        static const char* getVariantTypeString(EVariantType type);

    private:

        Bool*   getBoolRaw() const;
        Int*    getIntRaw() const;
        Float*  getFloatRaw() const;
        String* getStringRaw() const;
        Array*  getArrayRaw() const;
        Map*   &getMapRaw() const;
        void*   getData() const;

        template <typename ... TArgs>
        struct Max;

        template <typename T>
        struct Max<T> {
            static const size_t max = sizeof(T);
        };

        template <typename T, typename ... TArgs>
        struct Max<T,TArgs...> {
            static const size_t max = sizeof(T) > Max<TArgs...>::max ? sizeof(T) : Max<TArgs...>::max;
        };

        uint8 mData[Max<Bool,Int,Float,String,Array,Map*>::max] = {};
        EVariantType mType = EVariantType::Null;

    };

}

#endif //BERSERK_VARIANT_H