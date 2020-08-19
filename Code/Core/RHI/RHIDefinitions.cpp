/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RHI/RHIDefinitions.h>
#include <String/CString.h>

namespace Berserk {

    EVertexElementType RHIDefinitionsUtil::getElementTypeFromString(const Berserk::CString &type) {
        const char* const typeStrings[] = {
                "Float1", "Float2", "Float3", "Float4",
                "Int1", "Int2", "Int3", "Int4"
        };

        const EVertexElementType types[] = {
                EVertexElementType::Float1, EVertexElementType::Float2, EVertexElementType::Float3, EVertexElementType::Float4,
                EVertexElementType::Int1, EVertexElementType::Int2, EVertexElementType::Int3, EVertexElementType::Int4
        };

        for (int32 i = 0; i < sizeof(types)/ sizeof(types[0]); i++) {
            if (type == typeStrings[i]) {
                return types[i];
            }
        }

        return EVertexElementType::Unknown;
    }

    EVertexIterating RHIDefinitionsUtil::getIteratingFromString(const Berserk::CString &iterating) {
        if (iterating == "PerInstance") {
            return EVertexIterating::PerInstance;
        }

        return EVertexIterating::PerVertex;
    }

    uint32 RHIDefinitionsUtil::getIndexSize(Berserk::EIndexType type) {
        switch (type) {
            case EIndexType::Uint32:
                return sizeof(uint32);
            default:
                BERSERK_ERROR_RET_VALUE(0, "Unknown index type");
        }
    }

    uint32 RHIDefinitionsUtil::getVertexElementSize(Berserk::EVertexElementType elementType) {
        switch (elementType) {
            case EVertexElementType::Float1:
                return sizeof(float) * 1;
            case EVertexElementType::Float2:
                return sizeof(float) * 2;
            case EVertexElementType::Float3:
                return sizeof(float) * 3;
            case EVertexElementType::Float4:
                return sizeof(float) * 4;
            case EVertexElementType::Int1:
                return sizeof(int32) * 1;
            case EVertexElementType::Int2:
                return sizeof(int32) * 2;
            case EVertexElementType::Int3:
                return sizeof(int32) * 3;
            case EVertexElementType::Int4:
                return sizeof(int32) * 4;
            default:
                return 0;
        }
    }

    EShaderLanguage RHIDefinitionsUtil::getLanguageFromString(const CString &lang) {
        EShaderLanguage language = EShaderLanguage::Undefined;

        if (lang == "GLSL" || lang == "Glsl" || lang == "glsl") {
            language = EShaderLanguage::GLSL;
            return language;
        }

        return language;
    }

    EShaderType RHIDefinitionsUtil::getShaderTypeFromString(const Berserk::CString &type) {
        if (type == "Vertex")
            return EShaderType::Vertex;

        if (type == "Fragment")
            return EShaderType::Fragment;

        return EShaderType::Unknown;
    }

    const char* RHIDefinitionsUtil::getShaderTypeStringFromEnum(Berserk::EShaderType type) {
        switch (type) {
            case EShaderType::Vertex:
                return "Vertex";
            case EShaderType::Fragment:
                return "Fragment";
            default:
                return "Unknown";
        }
    }

    const char* RHIDefinitionsUtil::getVertexElementStringFromEnum(Berserk::EVertexElementType elementType) {
        const char* const typeStrings[] = {
                "Float1", "Float2", "Float3", "Float4",
                "Int1", "Int2", "Int3", "Int4"
        };

        const EVertexElementType types[] = {
                EVertexElementType::Float1, EVertexElementType::Float2, EVertexElementType::Float3, EVertexElementType::Float4,
                EVertexElementType::Int1, EVertexElementType::Int2, EVertexElementType::Int3, EVertexElementType::Int4
        };

        for (int32 i = 0; i < sizeof(types)/ sizeof(types[0]); i++) {
            if (elementType == types[i]) {
                return typeStrings[i];
            }
        }

        return "Unknown";
    }

    const char* RHIDefinitionsUtil::getVertexElementStringAsGlslType(EVertexElementType elementType) {
        const char* const typeStrings[] = {
                "float", "vec2", "vec3", "vec4",
                "int", "ivec2", "ivec3", "ivec4"
        };

        const EVertexElementType types[] = {
                EVertexElementType::Float1, EVertexElementType::Float2, EVertexElementType::Float3, EVertexElementType::Float4,
                EVertexElementType::Int1, EVertexElementType::Int2, EVertexElementType::Int3, EVertexElementType::Int4
        };

        for (int32 i = 0; i < sizeof(types)/ sizeof(types[0]); i++) {
            if (elementType == types[i]) {
                return typeStrings[i];
            }
        }

        return "Unknown";
    }

    const char* RHIDefinitionsUtil::getVertexIteratingStringFromEnum(Berserk::EVertexIterating iterating) {
        switch (iterating) {
            case EVertexIterating::PerVertex:
                return "PerVertex";
            case EVertexIterating::PerInstance:
                return "PerInstance";
            default:
                return "Unknown";
        }
    }

    const char * RHIDefinitionsUtil::getShaderDataStringFromEnum(EShaderData type) {
        switch (type) {
            case EShaderData::Float1:
                return "Float1";
            case EShaderData::Float2:
                return "Float2";
            case EShaderData::Float3:
                return "Float3";
            case EShaderData::Float4:
                return "Float4";
            case EShaderData::Int1:
                return "Int1";
            case EShaderData::Int2:
                return "Int2";
            case EShaderData::Int3:
                return "Int3";
            case EShaderData::Int4:
                return "Int4";
            case EShaderData::Bool1:
                return "Bool1";
            case EShaderData::Bool2:
                return "Bool2";
            case EShaderData::Bool3:
                return "Bool3";
            case EShaderData::Bool4:
                return "Bool4";
            case EShaderData::Mat2:
                return "Mat2";
            case EShaderData::Mat3:
                return "Mat3";
            case EShaderData::Mat4:
                return "Mat4";
            case EShaderData::Sampler2D:
                return "Sampler2D";
            case EShaderData::Sampler3D:
                return "Sampler3D";
            case EShaderData::SamplerCube:
                return "SamplerCube";
            default:
                return "Undefined";
        }
    }

    TArrayStatic<EShaderType> RHIDefinitionsUtil::getShaderTypes() {
        return { EShaderType::Vertex, EShaderType::Fragment };
    }

    TArrayStatic<CString> RHIDefinitionsUtil::getShaderTypesAsString() {
        return { "Vertex", "Fragment" };
    }

}