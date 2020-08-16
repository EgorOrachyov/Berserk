/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Resources/MeshAttribute.h>

namespace Berserk {

    static const char* names[] = { "Position", "Normal", "Tangent", "Color", "TexCoords" };

    static const EVertexElementType elements[] = { EVertexElementType::Float3, EVertexElementType::Float3, EVertexElementType::Float3, EVertexElementType::Float3, EVertexElementType::Float2 };

    static const EMeshAttribute types[] = { EMeshAttribute::Position, EMeshAttribute::Normal, EMeshAttribute::Tangent, EMeshAttribute::Color, EMeshAttribute::TexCoord };

    static const uint32 attributesCount = sizeof(types)/ sizeof(types[0]);

    CString MeshFormatUtil::getFormatAsString(Berserk::MeshFormat format) {
        CString str = "[";

        bool appendComma = false;

        for (uint32 i = 0; i < attributesCount; i++) {
            if (format.getFlag(types[i])) {
                if (appendComma) {
                    str += ",";
                }

                str += names[i];
                appendComma = true;
            }
        }

        return str + "]";
    }

    const char* MeshFormatUtil::getAttributeNameFromEnum(EMeshAttribute attribute) {
        for (uint32 i = 0; i < attributesCount; i++) {
            if (attribute == types[i])
                return names[i];
        }

        return "Unknown";
    }

    uint32 MeshFormatUtil::getAttributeSize(EMeshAttribute attribute) {
        for (uint32 i = 0; i < attributesCount; i++) {
            if (attribute == types[i])
                return RHIDefinitionsUtil::getVertexElementSize(elements[i]);
        }

        return 0;
    }

    EVertexElementType MeshFormatUtil::getAttributeType(EMeshAttribute attribute) {
        for (uint32 i = 0; i < attributesCount; i++) {
            if (attribute == types[i])
                return elements[i];
        }

        return EVertexElementType::Unknown;
    }

    uint32 MeshFormatUtil::getStrideSizeForFormat(const MeshFormat &format) {
        uint32 stride = 0;

        for (auto type: types) {
            if (format.getFlag(type)) {
                stride += getAttributeSize(type);
            }
        }

        return stride;
    }

}