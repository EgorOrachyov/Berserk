/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHISHADERATTRIBUTE_H
#define BERSERK_RHISHADERATTRIBUTE_H

#include <Containers/TMap.h>
#include <Containers/TArray.h>
#include <TEnumMask.h>
#include <RHI/RHIDefinitions.h>

namespace Berserk {

    /** Input/input shader attribute */
    class ShaderAttribute {
    public:

        ShaderAttribute() = default;
        ShaderAttribute(CString name, uint32 location, EVertexElementType dataType) {
            mName = std::move(name);
            mLocation = location;
            mData = dataType;
        }

        /** @return Name of the attribute in the shader */
        const CString& getName() const { return mName; }

        /** @return Native location for shader input/output */
        uint32 getLocation() const { return mLocation; }

        /** @return Type of the param */
        EVertexElementType getDataType() const { return mData; }

        /** Serialization for shader caching */
        friend Archive& operator<<(Archive& archive, const ShaderAttribute& attribute) {
            archive << attribute.mName;
            archive << attribute.mLocation;
            archive << (uint32)attribute.mData;
            return archive;
        }

        /** Deserialization for shader caching */
        friend Archive& operator>>(Archive& archive, ShaderAttribute& attribute) {
            archive >> attribute.mName;
            archive >> attribute.mLocation;
            archive >> (uint32&)attribute.mData;
            return archive;
        }

    private:
        CString mName;
        uint32 mLocation;
        EVertexElementType mData;
    };

}

#endif //BERSERK_RHISHADERATTRIBUTE_H
