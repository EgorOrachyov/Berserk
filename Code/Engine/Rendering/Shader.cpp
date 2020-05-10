/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/Shader.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    
    Shader::Shader(CString name, EShaderLanguage language, const TArray<uint8> &vertex, const TArray<uint8> &fragment) {
        mName = std::move(name);
        mLanguage = language;

        auto& device = RHIDevice::getSingleton();
        auto& langs = device.getSupportedShaderLanguages();

        BERSERK_COND_ERROR_RET(langs.contains(language), "Device does not support provided shader sources language");

        RHIShaderViewDesc shaderDesc;
        shaderDesc.resize(2);
        shaderDesc[0].code = &vertex;
        shaderDesc[0].type = EShaderType::Vertex;
        shaderDesc[1].code = &fragment;
        shaderDesc[1].type = EShaderType::Fragment;

        mShaderHandle = std::move(device.createShader(language, shaderDesc));
        if (mShaderHandle.isNotNull()) mMetaData = std::move(device.createShaderIntrospection(mShaderHandle));
    }

    bool Shader::isValid() const {
        return mShaderHandle.isNotNull() && mMetaData.isNotNull();
    }
}