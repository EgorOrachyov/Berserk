/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GPUMESHFORMAT_H
#define BERSERK_GPUMESHFORMAT_H

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
        class MeshFormat: public TEnumMask<EMeshAttribute> {
        public:
            using TEnumMask<EMeshAttribute>::TEnumMask;

            /** @return Attributes in this format */
            TArrayStatic<EMeshAttribute,(uint32)EMeshAttribute::Max> getAttributes() const;

            /** @return Packed stride for this mesh format */
            uint32 getStride() const;

            /** @return To human readable string */
            CString toString() const;

            /** @return To string as vertex declaration name */
            CString toStringAsDeclarationName() const;
        };

        class MeshFormatUtil {
        public:
            static const char* getAttributeNameFromEnum(EMeshAttribute attribute);
            static const char* getAttributeDeclarationNameFromEnum(EMeshAttribute attribute);
            static uint32 getAttributeSize(EMeshAttribute attribute);
            static EVertexElementType getAttributeType(EMeshAttribute attribute);
        };

    }
}

#endif //BERSERK_GPUMESHFORMAT_H
