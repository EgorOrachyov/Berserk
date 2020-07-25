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
        switch (elementType) {
            case EVertexElementType::Float1:
                return "Float1";
            case EVertexElementType::Float2:
                return "Float2";
            case EVertexElementType::Float3:
                return "Float3";
            case EVertexElementType::Float4:
                return "Float4";
            default:
                return "Unknown";
        }
    }

    TArrayStatic<EShaderType> RHIDefinitionsUtil::getShaderTypes() {
        return { EShaderType::Vertex, EShaderType::Fragment };
    }

    TArrayStatic<CString> RHIDefinitionsUtil::getShaderTypesAsString() {
        return { "Vertex", "Fragment" };
    }

}