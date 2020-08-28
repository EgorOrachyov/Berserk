/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <GpuMeshFormat.h>

namespace Berserk {
    namespace Render {

        static const char* names[] = { "Position", "Normal", "Tangent", "Color", "TexCoords" };

        static const char* declarationNames[] = { "inPosition", "inNormal", "inTangent", "inColor", "inTexCoords" };

        static const EVertexElementType elements[] = { EVertexElementType::Float3, EVertexElementType::Float3, EVertexElementType::Float3, EVertexElementType::Float3, EVertexElementType::Float2 };

        static const EMeshAttribute types[] = { EMeshAttribute::Position, EMeshAttribute::Normal, EMeshAttribute::Tangent, EMeshAttribute::Color, EMeshAttribute::TexCoord };

        static const uint32 attributesCount = sizeof(types)/ sizeof(types[0]);

        TArrayStatic<EMeshAttribute,(uint32)EMeshAttribute::Max> MeshFormat::getAttributes() const {
            TArrayStatic<EMeshAttribute,(uint32)EMeshAttribute::Max> result;

            for (auto type: types) {
                if (getFlag(type))
                    result.add(type);
            }

            return result;
        }

        uint32 MeshFormat::getStride() const {
            uint32 stride = 0;

            for (auto type: types) {
                if (getFlag(type)) {
                    stride += MeshFormatUtil::getAttributeSize(type);
                }
            }

            return stride;
        }

        CString MeshFormat::toString() const {
            CString str = "[";

            bool appendComma = false;

            for (uint32 i = 0; i < attributesCount; i++) {
                if (getFlag(types[i])) {
                    if (appendComma) {
                        str += ",";
                    }

                    str += names[i];
                    appendComma = true;
                }
            }

            return str + "]";
        }

        CString MeshFormat::toStringAsDeclarationName() const {
            CString str;

            bool appendDelimiter = false;

            for (uint32 i = 0; i < attributesCount; i++) {
                if (getFlag(types[i])) {
                    if (appendDelimiter) {
                        str += ".";
                    }

                    str += declarationNames[i];
                    appendDelimiter = true;
                }
            }

            return str;
        }

        const char* MeshFormatUtil::getAttributeNameFromEnum(EMeshAttribute attribute) {
            for (uint32 i = 0; i < attributesCount; i++) {
                if (attribute == types[i])
                    return names[i];
            }

            return "Unknown";
        }

        const char* MeshFormatUtil::getAttributeDeclarationNameFromEnum(EMeshAttribute attribute) {
            for (uint32 i = 0; i < attributesCount; i++) {
                if (attribute == types[i])
                    return declarationNames[i];
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

    }
}