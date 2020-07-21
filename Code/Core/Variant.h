/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VARIANT_H
#define BERSERK_VARIANT_H

#include <Containers/TMap.h>
#include <Containers/TArray.h>
#include <IO/Archive.h>
#include <String/CString.h>

namespace Berserk {

    /** Supported variants type */
    enum class EVariantType {
        Null    = 0,
        Bool    = 1,
        Int     = 2,
        Float   = 4,
        String  = 5,
        Array   = 6,
        Map     = 7
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
        using Object = Map;                 // Alias - the same for some languages

        Variant() = default;

        Variant(Bool value);
        Variant(Int value);
        Variant(Float value);
        Variant(const char *string);
        Variant(const String &string);
        Variant(const CStringStatic &string);
        Variant(const std::initializer_list<Variant> &array);
        Variant(const Array &array);
        Variant(const Map &map);

        Variant(const Variant& other);
        Variant(Variant&& other) noexcept;

        ~Variant();

        void asNull();
        void asBool();
        void asInt();
        void asFloat();
        void asString();
        void asArray();
        void asMap();
        void asObject() { asMap(); }

        operator Bool();
        operator Int();
        operator Float();
        operator String();
        operator Array();
        operator Map();

        Bool&   getBool();
        Int&    getInt();
        Float&  getFloat();
        String& getString();
        Array&  getArray();
        Map&    getMap();
        Object& getObject() { return getMap(); }

        bool isNull() const { return mType == EVariantType::Null; }
        bool isBool() const { return mType == EVariantType::Bool; }
        bool isInt() const { return mType == EVariantType::Int; }
        bool isFloat() const { return mType == EVariantType::Float; }
        bool isString() const { return mType == EVariantType::String; }
        bool isArray() const { return mType == EVariantType::Array; }
        bool isMap() const { return mType == EVariantType::Map; }
        bool isObject() const { return isMap(); }
        bool isNotNull() const { return mType != EVariantType::Null; }

        uint32 hash() const;
        EVariantType getType() const { return mType; }
        CString toString() const;
        void toStringBuilder(class CStringBuilder& builder) const;

        Variant& operator=(const Variant& other);
        Variant& operator=(Variant&& other) noexcept;

        bool operator==(const Variant& variant) const;
        bool operator!=(const Variant& variant) const;

        friend Archive& operator<<(Archive& archive, const Variant& variant);
        friend Archive& operator>>(Archive& archive, Variant& variant);

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
        struct Max<T> { static const size_t max = sizeof(T); };

        template <typename T, typename ... TArgs>
        struct Max<T,TArgs...> { static const size_t max = sizeof(T) > Max<TArgs...>::max ? sizeof(T) : Max<TArgs...>::max; };

        uint8 mData[Max<Bool,Int,Float,String,Array,Map*>::max] = {};
        EVariantType mType = EVariantType::Null;

    };

}

#endif //BERSERK_VARIANT_H