/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADER_H
#define BERSERK_SHADER_H

#include <RHI/RHIResources.h>
#include <RHI/RHIShaderMetaData.h>
#include <Rendering/VertexPolicy.h>
#include <Rendering/Resources/IRenderResource.h>

namespace Berserk {
    namespace Rendering {

        /**
         * @brief Shader program
         * Compiled shader program and its params info for the rendering thread
         */
        class Shader : public IRenderResource {
        public:
            Shader(CString name, EShaderLanguage language, const TArray<uint8> &vertex, const TArray<uint8> &fragment);
            Shader() = default;
            ~Shader() override;

            bool isInitialized() const override;
            bool isInitializedRHI() const override;
            CString getFriendlyName() const override;

            /** @return Shader param by name if present */
            TRef<const ShaderParam> findParam(const CString &name) const { return mMetaData->findParam(name); }
            /** @return Shader uniform block by name if present */
            TRef<const ShaderUniformBlock> findUniformBlock(const CString &name) const { return mMetaData->findUniformBlock(name); }

            const CString &getName() const { return mShaderName; }
            const EShaderLanguage &getLanguage() const { return mLanguage; }
            const TPtrShared<RHIShader> &getShaderHandle() const { return mShaderHandle; }
            const TPtrShared<RHIShaderMetaData> &getMetaData() const { return mMetaData; }

            virtual void serialize(Archive& archive);
            virtual void deserialize(Archive& archive);

        protected:
            CString mShaderName;
            TPtrShared<RHIShader> mShaderHandle;
            TPtrShared<RHIShaderMetaData> mMetaData;
            EShaderLanguage mLanguage = EShaderLanguage::Undefined;
        };

    }
}

#endif //BERSERK_SHADER_H