/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADER_H
#define BERSERK_SHADER_H

#include <Containers/BinaryData.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIShaderMetaData.h>
#include <VertexPolicy.h>
#include <Resources/RenderResource.h>

namespace Berserk {
    namespace Rendering {

        /**
         * @brief Shader program
         * Compiled shader program and its params info for the rendering thread
         */
        class Shader : public RenderResource {
        public:
            /**
             * Create vertex+fragment shader program from specified language source code.
             * @param name Descriptive global shader look-up name
             * @param language Language of the sources
             * @param vertex Vertex shader source code
             * @param fragment Fragment shader source code
             * @param cacheCode True if want to cache sources on CPU (useful for serialization)
             */
            Shader(CString name, EShaderLanguage language, const TArray<uint8> &vertex, const TArray<uint8> &fragment, bool cacheCode = false);
            Shader() = default;
            ~Shader() override;

            bool isInitialized() const override;
            bool isInitializedRHI() const override;
            CString getFriendlyName() const override;

            /** @return Shader param by name if present */
            TRef<const ShaderParam> findParam(const CString &name) const { return mMetaData->findParam(name); }

            /** @return Shader uniform block by name if present */
            TRef<const ShaderUniformBlock> findUniformBlock(const CString &name) const { return mMetaData->findUniformBlock(name); }

            /** @return Shader global look-up name (the same for friendly name) */
            const CString &getName() const { return mShaderName; }

            /** @return Shader language of source */
            const EShaderLanguage &getLanguage() const { return mLanguage; }

            /** @return Shader RHI handle (possibly null) */
            const TPtrShared<RHIShader> &getShaderHandle() const { return mShaderHandle; }

            /** @return Shader meta data RHI handle (possibly null) */
            const TPtrShared<RHIShaderMetaData> &getMetaData() const { return mMetaData; }

            /** @return True if shader could be cached by shader cache */
            virtual bool supportsSerialization() const;

            /** Serialize shader in raw archive for future read */
            virtual bool serialize(Archive &archive) const;

            /** Deserialize ready-for usage shader from archive  */
            virtual bool deserialize(Archive &archive);

        protected:
            /** Descriptive name */
            CString mShaderName;
            /** Handle to actual RHI */
            TPtrShared<RHIShader> mShaderHandle;
            /** Handle to actual RHI meta */
            TPtrShared<RHIShaderMetaData> mMetaData;
            /** Source language for this shader */
            EShaderLanguage mLanguage = EShaderLanguage::Undefined;
            /** Type of the target device this shader compiled */
            ERenderDeviceType mDeviceType = ERenderDeviceType::Undefined;
            /** Cached sources */
            TArray<TPair<uint32, BinaryData>> mCachedCode;
            /** True if must cache sources on CPU */
            bool mCacheCode = false;
            /** True if shader was loaded from binary cache (supported by some RHI) */
            bool mFromBinaryCache = false;
        };

    }
}

#endif //BERSERK_SHADER_H