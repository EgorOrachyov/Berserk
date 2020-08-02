/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_PROPERTY_H
#define BERSERK_PROPERTY_H

#include <Variant.h>
#include <TEnumMask.h>

namespace Berserk {

    enum class EPropertyFlags {

    };

    class Property {
    public:

        Property(CString className, TEnumMask<EPropertyFlags> flags = {}, CString hint = "") {
            mClassName = std::move(className);
            mHint = std::move(hint);
            mFlags = flags;
        }

        Property(EVariantType type, TEnumMask<EPropertyFlags> flags = {}, CString hint = "") {
            mType = type;
            mHint = std::move(hint);
            mFlags = flags;
        }

        Property(CString name, CString className, TEnumMask<EPropertyFlags> flags = {}, CString hint = "") {
            mName = std::move(name);
            mClassName = std::move(className);
            mHint = std::move(hint);
            mFlags = flags;
        }

        Property(CString name, EVariantType type, TEnumMask<EPropertyFlags> flags = {}, CString hint = "") {
            mName = std::move(name);
            mType = type;
            mHint = std::move(hint);
            mFlags = flags;
        }

        Property() = default;

        /** @return True if properties has the same type at all */
        bool isSameType(const Property& other) const {
            if (isClassType() && other.isClassType())
                return getClassName() == other.getClassName();

            return getTypeVariant() == other.getTypeVariant();
        }

        /** @return True if property of the some class type */
        bool isClassType() const { return mType == EVariantType::Null; }

        /** @return Type of the property if it has base type */
        EVariantType getTypeVariant() const { return mType; }

        /** @return Property name */
        const CString& getName() const { return mName; }

        /** @return Property class name (if property has class type) */
        const CString& getClassName() const { return mClassName; }

        /** @return Hint message info */
        const CString& getHintMessage() const { return mHint; }

        /** @return Property flags */
        TEnumMask<EPropertyFlags> getFlags() const { return mFlags; }

        /** @return Property type as string for output */
        const char* getTypeAsString() const { return isClassType()? getClassName().data(): Variant::getVariantTypeString(mType); }

        /** Print debug info about property */
        void showDebugInfo() const {
            printf("  Name: %s, type: %s, hint: %s\n",
                    getName().data(),
                    getTypeAsString(),
                    getHintMessage().data());
        }

    private:
        template <typename T>
        friend class ClassBuilder;

        CString mClassName;
        CString mName;
        CString mHint;
        EVariantType mType = EVariantType::Null;
        TEnumMask<EPropertyFlags> mFlags;

    };

}

#endif //BERSERK_PROPERTY_H