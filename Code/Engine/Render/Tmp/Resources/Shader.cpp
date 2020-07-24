/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Tmp/Resources/Shader.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Rendering {

        Shader::Shader(CString name, EShaderLanguage language,
                       const TArray <uint8> &vertex,
                       const TArray <uint8> &fragment,
                       bool cacheCode) {
            mShaderName = std::move(name);
            mLanguage = language;
            mCacheCode = cacheCode;

            auto &device = RHIDevice::getSingleton();
            auto &shaderLanguages = device.getSupportedShaderLanguages();

            BERSERK_COND_ERROR_RET(shaderLanguages.contains(language), "Device does not support provided shader sources language");

            RHIShaderViewDesc shaderDesc;
            shaderDesc.resize(2);
            shaderDesc[0].code = &vertex;
            shaderDesc[0].type = EShaderType::Vertex;
            shaderDesc[1].code = &fragment;
            shaderDesc[1].type = EShaderType::Fragment;

            mDeviceType = device.getDeviceType();
            mShaderHandle = std::move(device.createShader(language, shaderDesc));
            if (mShaderHandle.isNotNull()) mMetaData = std::move(device.createShaderMeta(mShaderHandle));

            if (mCacheCode) {
                TPair<uint32,BinaryData> vsEntry;
                vsEntry.first() = (uint32) EShaderType::Vertex;
                vsEntry.second().add(vertex.data(), vertex.size());

                TPair<uint32,BinaryData> fsEntry;
                fsEntry.first() = (uint32) EShaderType::Fragment;
                fsEntry.second().add(fragment.data(), fragment.size());

                mCachedCode.ensureToAdd(2);
                mCachedCode.move(vsEntry);
                mCachedCode.move(fsEntry);
            }

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

        bool Shader::supportsSerialization() const {
            auto canSerializeRHI = mShaderHandle.isNotNull() ? mShaderHandle->supportsSerialization() : false;
            auto canSerialize = mCacheCode || canSerializeRHI;
            return canSerialize && mMetaData.isNotNull();
        }

        bool Shader::serialize(Archive &archive) const {
            BERSERK_COND_ERROR_RET_VALUE(false, supportsSerialization(), "Cannot serialize shader '%s'", mShaderName.data());

            auto canSerializeRHI = mShaderHandle.isNotNull() ? mShaderHandle->supportsSerialization() : false;
            auto canSerializeCode = mCacheCode;

            if (canSerializeRHI) {
                BinaryData binary;

                // Note: some serialization operations can fail
                auto result = mShaderHandle->serialize(binary);

                if (result) {
                    bool serializeBinary = true;
                    bool cacheCode = false;

                    archive << mShaderName;
                    archive << (uint32) mLanguage;
                    archive << (uint32) mDeviceType;
                    archive << *mMetaData;
                    archive << serializeBinary;
                    archive << binary;
                    archive << cacheCode; // Explicitly tell that no code

                    return true;
                }
            }

            if (canSerializeCode) {
                bool serializeBinary = false;
                bool cacheCode = true;

                archive << mShaderName;
                archive << (uint32) mLanguage;
                archive << (uint32) mDeviceType;
                archive << *mMetaData;
                archive << serializeBinary;
                archive << cacheCode;
                archive << mCachedCode;

                return true;
            }

            BERSERK_ERROR("Failed to serialize shader '%s'", mShaderName.data())
            return false;
        }

        bool Shader::deserialize(Archive &archive) {
            auto result = true;
            auto& device = RHIDevice::getSingleton();
            auto &shaderLanguages = device.getSupportedShaderLanguages();

            bool serializeBinary = false;
            bool cacheCode = false;

            // Empty meta to read and store data
            mMetaData = device.createShaderMeta();

            archive >> mShaderName;
            archive >> (uint32&) mLanguage;
            archive >> (uint32&) mDeviceType;
            archive >> *mMetaData;
            archive >> serializeBinary;

            if (serializeBinary) {
                mFromBinaryCache = true;
                // todo: read binary and create shader from binary cache
            }

            archive >> cacheCode;

            if (cacheCode) {
                mCacheCode = true;
                archive >> mCachedCode;

                RHIShaderViewDesc shaderDesc;
                shaderDesc.resize(2);
                shaderDesc[0].code = &mCachedCode[0].second().internal();
                shaderDesc[0].type = (EShaderType) mCachedCode[0].first();
                shaderDesc[1].code = &mCachedCode[1].second().internal();
                shaderDesc[1].type = (EShaderType) mCachedCode[1].first();

                mShaderHandle = device.createShader(mLanguage, shaderDesc);
                if (mShaderHandle.isNotNull()) linkResource();

                result = mShaderHandle.isNotNull();
                BERSERK_COND_ERROR(result, "Failed to create shader '%s' from sources", mShaderName.data());
            }

            return result;
        }

    }
}