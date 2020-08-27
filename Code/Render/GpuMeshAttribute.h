/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GPUMESHATTRIBUTE_H
#define BERSERK_GPUMESHATTRIBUTE_H

#include <TEnumMask.h>
#include <String/CString.h>
#include <RHI/RHIDefinitions.h>

namespace Berserk {
    namespace Render {

        /** Fixed mesh vertex attributes */
        enum class EMeshAttribute {
            /** Vec3f position */
            Position = 0,
            /** Vec3f vertex normal vector */
            Normal = 1,
            /** Vec3f vertex tangent (bitangent evaluated in the vertex shader) */
            Tangent = 2,
            /** Vec3f optional per vertex color */
            Color = 3,
            /** Vec2f texture coordinates */
            TexCoord = 4,
            /** On error */
            Unknown = 0xff,
            /** Total number of known attributes */
            Max = 5,
        };

        /** Mesh vertex format type */
        using MeshFormat = TEnumMask<EMeshAttribute>;

        class MeshFormatUtil {
        public:
            static CString getFormatAsString(MeshFormat format);
            static CString getFormatAsDeclarationString(MeshFormat format);
            static const char* getAttributeNameFromEnum(EMeshAttribute attribute);
            static const char* getAttributeDeclarationNameFromEnum(EMeshAttribute attribute);
            static uint32 getStrideSizeForFormat(const MeshFormat& format);
            static uint32 getAttributeSize(EMeshAttribute attribute);
            static EVertexElementType getAttributeType(EMeshAttribute attribute);
        };

    }
}

#endif //BERSERK_GPUMESHATTRIBUTE_H
