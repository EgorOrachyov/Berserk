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

}