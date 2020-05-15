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

    EShaderLanguage RHIDefinitionsUtil::getLanguageFromString(const CString &lang) {
        EShaderLanguage language = EShaderLanguage::Undefined;

        if (lang == "GLSL" || lang == "Glsl" || lang == "glsl") {
            language = EShaderLanguage::GLSL;
            return language;
        }

        return language;
    }

}