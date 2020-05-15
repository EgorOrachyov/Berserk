/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/Resources/Shader.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Rendering {

        Shader::Shader(CString name, EShaderLanguage language, const TArray<uint8> &vertex, const TArray<uint8> &fragment) {
            mShaderName = std::move(name);
            mLanguage = language;

            auto &device = RHIDevice::getSingleton();
            auto &langs = device.getSupportedShaderLanguages();

            BERSERK_COND_ERROR_RET(langs.contains(language), "Device does not support provided shader sources language");

            RHIShaderViewDesc shaderDesc;
            shaderDesc.resize(2);
            shaderDesc[0].code = &vertex;
            shaderDesc[0].type = EShaderType::Vertex;
            shaderDesc[1].code = &fragment;
            shaderDesc[1].type = EShaderType::Fragment;

            mShaderHandle = std::move(device.createShader(language, shaderDesc));
            if (mShaderHandle.isNotNull()) mMetaData = std::move(device.createShaderIntrospection(mShaderHandle));

            linkResource();
        }

        Shader::~Shader() {
            unlinkResource();
        }

        bool Shader::isInitialized() const {
            return isInitializedRHI();
        }

        bool Shader::isInitializedRHI() const {
            return mShaderHandle.isNotNull() && mMetaData.isNotNull();
        }

        CString Shader::getFriendlyName() const {
            return mShaderName;
        }

        void Shader::serialize(Berserk::Archive &archive) {

        }

        void Shader::deserialize(Berserk::Archive &archive) {

        }

    }
}