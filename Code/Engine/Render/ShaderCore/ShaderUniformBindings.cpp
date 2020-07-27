/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ShaderCore/ShaderUniformBindings.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Render {

        ShaderUniformBindings::ShaderUniformBindings(TPtrShared<RHIShaderMetaData> metaData) {
            BERSERK_COND_ERROR_RET(metaData.isNotNull(), "Null meta data passed");

            mMetaData = std::move(metaData);
            mTextures.resize(mMetaData->getParams().size());
            mUniformBlocks.resize(mMetaData->getUniformBlocks().size());

            auto& blocks = mMetaData->getUniformBlocks();
            for (uint32 i = 0; i < blocks.size(); i++) {
                mUniformBlocks[i].binding = blocks[i].getBinding();
            }

            auto& params = mMetaData->getParams();
            for (uint32 i = 0; i < params.size(); i++) {
                mTextures[i].location = params[i].getLocation();
            }
        }

        void ShaderUniformBindings::associateUniformBuffer(const CString &blockName, uint32 offset, TPtrShared<UniformBuffer> buffer) {
            BERSERK_COND_ERROR_RET(buffer.isNotNull(), "Uniform buffer must not be null");

            auto& blocks = mMetaData->getUniformBlocks();

            for (uint32 i = 0; i < blocks.size(); i++) {
                if (blocks[i].getName() == blockName) {
                    auto range = blocks[i].getSize();

                    BERSERK_COND_ERROR_RET(buffer->isValidRange(offset, range), "Invalid range for block: %s", blockName.data());

                    auto& uniformBlock = mUniformBlocks[i];
                    uniformBlock.buffer = std::move(buffer);
                    uniformBlock.offset = offset;
                    uniformBlock.range = range;
                    return;
                }
            }

            BERSERK_ERROR("No such uniform block: %s", blockName.data());
        }

        void ShaderUniformBindings::associateUniformBuffer(const CString &blockName, TPtrShared<UniformBuffer> buffer) {
            associateUniformBuffer(blockName, 0, std::move(buffer));
        }

        void ShaderUniformBindings::associateUniformBuffer(const CString &blockName) {
            auto block = mMetaData->findUniformBlock(blockName);

            if (block.isNotNull()) {
                auto size = block->getSize();
                auto buffer = TPtrShared<UniformBuffer>::make(size);

                associateUniformBuffer(blockName, buffer);
            }

            BERSERK_ERROR("No such uniform block: %s", blockName.data());
        }

        void ShaderUniformBindings::associateUniformBuffers() {
            auto& blocks = mMetaData->getUniformBlocks();

            for (uint32 i = 0; i < blocks.size(); i++) {
                auto size = blocks[i].getSize();
                auto buffer = TPtrShared<UniformBuffer>::make(size);

                auto& uniformBlock = mUniformBlocks[i];
                uniformBlock.buffer = std::move(buffer);
                uniformBlock.offset = 0;
                uniformBlock.range = size;
            }
        }

        bool ShaderUniformBindings::isDirty() const {
            return mIsDirtyTextures || mIsDirtyUniformBlocks || mIsUniformBuffersDataDirty;
        }

        bool ShaderUniformBindings::isSetDirty() const {
            return mIsDirtyTextures || mIsDirtyUniformBlocks;
        }

        ShaderUniformBindings::TextureParam ShaderUniformBindings::findTexture2D(const CString &texture) const {
            TextureParam param;
            param.index = -1;

            auto& params = mMetaData->getParams();

            int32 index = 0;
            for (auto& p: params) {
                if (p.getName() == texture && p.getDataType() == EShaderData::Sampler2D) {
                    param.index = index;
                    param.param = &p;
                    return param;
                }

                index += 1;
            }

            return param;
        }

        ShaderUniformBindings::BlockParam ShaderUniformBindings::findParam(const CString &uniformBlock, const CString &paramName) const {
            BlockParam param;
            param.index = -1;

            auto& blocks = mMetaData->getUniformBlocks();

            int32 blockIndex = 0;
            for (auto& b: blocks) {
                if (b.getName() == uniformBlock) {
                    auto member = b.findMember(paramName);

                    if (member.isNotNull()) {
                        param.index = blockIndex;
                        param.param = member.getPtr();
                    }

                    return param;
                }

                blockIndex += 1;
            }

            return param;
        }

        void ShaderUniformBindings::setTexture2D(const ShaderUniformBindings::TextureParam &param, const TPtrShared<Texture> &t) {
            BERSERK_COND_ERROR_RET(param.index >= 0 && param.index < mTextures.size(), "Invalid param index");
            BERSERK_COND_ERROR_RET(param.param->getDataType() == EShaderData::Sampler2D, "Invalid param type");

            auto& texture = mTextures[param.index];

            if (texture.texture != t) {
                mIsDirtyTextures = true;
                texture.texture = t;
            }
        }
        
        void ShaderUniformBindings::setFloat(const ShaderUniformBindings::BlockParam &param, float t) {
            BERSERK_COND_ERROR_RET(param.index >= 0 && param.index < mUniformBlocks.size(), "Invalid param index");
            BERSERK_COND_ERROR_RET(param.param->getBaseType() == EShaderData::Float1, "Invalid param type");

            auto& block = mUniformBlocks[param.index];
            auto offset = block.offset + param.param->getOffset();

            block.buffer->setFloat(t, offset);

            mIsUniformBuffersDataDirty = true;
        }

        void ShaderUniformBindings::setVec2(const ShaderUniformBindings::BlockParam &param, const ShaderUniformBindings::Vec2 &t) {
            BERSERK_COND_ERROR_RET(param.index >= 0 && param.index < mUniformBlocks.size(), "Invalid param index");
            BERSERK_COND_ERROR_RET(param.param->getBaseType() == EShaderData::Float2, "Invalid param type");

            auto& block = mUniformBlocks[param.index];
            auto offset = block.offset + param.param->getOffset();

            block.buffer->setVec2(t, offset);

            mIsUniformBuffersDataDirty = true;
        }

        void ShaderUniformBindings::setVec3(const ShaderUniformBindings::BlockParam &param, const ShaderUniformBindings::Vec3 &t) {
            BERSERK_COND_ERROR_RET(param.index >= 0 && param.index < mUniformBlocks.size(), "Invalid param index");
            BERSERK_COND_ERROR_RET(param.param->getBaseType() == EShaderData::Float3, "Invalid param type");

            auto& block = mUniformBlocks[param.index];
            auto offset = block.offset + param.param->getOffset();

            block.buffer->setVec3(t, offset);

            mIsUniformBuffersDataDirty = true;
        }

        void ShaderUniformBindings::setVec4(const ShaderUniformBindings::BlockParam &param, const ShaderUniformBindings::Vec4 &t) {
            BERSERK_COND_ERROR_RET(param.index >= 0 && param.index < mUniformBlocks.size(), "Invalid param index");
            BERSERK_COND_ERROR_RET(param.param->getBaseType() == EShaderData::Float4, "Invalid param type");

            auto& block = mUniformBlocks[param.index];
            auto offset = block.offset + param.param->getOffset();

            block.buffer->setVec4(t, offset);

            mIsUniformBuffersDataDirty = true;
        }

        void ShaderUniformBindings::setMat2(const ShaderUniformBindings::BlockParam &param, const ShaderUniformBindings::Mat2 &t) {
            BERSERK_COND_ERROR_RET(param.index >= 0 && param.index < mUniformBlocks.size(), "Invalid param index");
            BERSERK_COND_ERROR_RET(param.param->getBaseType() == EShaderData::Mat2, "Invalid param type");

            auto& block = mUniformBlocks[param.index];
            auto offset = block.offset + param.param->getOffset();
            auto stride = param.param->getMatrixStride();
            auto transpose = !param.param->getIsRowMajor();

            block.buffer->setMat2(t, offset, stride, transpose);

            mIsUniformBuffersDataDirty = true;
        }

        void ShaderUniformBindings::setMat3(const ShaderUniformBindings::BlockParam &param, const ShaderUniformBindings::Mat3 &t) {
            BERSERK_COND_ERROR_RET(param.index >= 0 && param.index < mUniformBlocks.size(), "Invalid param index");
            BERSERK_COND_ERROR_RET(param.param->getBaseType() == EShaderData::Mat3, "Invalid param type");

            auto& block = mUniformBlocks[param.index];
            auto offset = block.offset + param.param->getOffset();
            auto stride = param.param->getMatrixStride();
            auto transpose = !param.param->getIsRowMajor();

            block.buffer->setMat3(t, offset, stride, transpose);

            mIsUniformBuffersDataDirty = true;
        }

        void ShaderUniformBindings::setMat4(const ShaderUniformBindings::BlockParam &param, const ShaderUniformBindings::Mat4 &t) {
            BERSERK_COND_ERROR_RET(param.index >= 0 && param.index < mUniformBlocks.size(), "Invalid param index");
            BERSERK_COND_ERROR_RET(param.param->getBaseType() == EShaderData::Mat4, "Invalid param type");

            auto& block = mUniformBlocks[param.index];
            auto offset = block.offset + param.param->getOffset();
            auto stride = param.param->getMatrixStride();
            auto transpose = !param.param->getIsRowMajor();

            block.buffer->setMat4(t, offset, stride, transpose);

            mIsUniformBuffersDataDirty = true;
        }
                                            
        void ShaderUniformBindings::updateSetGPU() {
            if (!isSetDirty())
                return;

            mUniformSetRHI.free();
            TArray<RHIUniformBlockDesc> blocksDesc;
            TArray<RHIUniformTextureDesc> texturesDesc;

            blocksDesc.ensureToAdd(mUniformBlocks.size());
            texturesDesc.ensureToAdd(mTextures.size());

            if (mIsDirtyTextures) {
                uint32 i = 0;
                for (auto& texture: mTextures) {
                    auto& desc = texturesDesc.emplace();

                    desc.location = texture.location;
                    desc.sampler = texture.texture->getSamplerRHI();
                    desc.texture = texture.texture->getTextureRHI();
                }
            }

            if (mIsDirtyUniformBlocks) {
                uint32 i = 0;
                for (auto& block: mUniformBlocks) {
                    auto& desc = blocksDesc.emplace();

                    desc.binding = block.binding;
                    desc.offset = block.offset;
                    desc.range = block.range;
                    desc.buffer = block.buffer->getRHI();
                }
            }

            mUniformSetRHI = RHIDevice::getSingleton().createUniformSet(texturesDesc, blocksDesc);
            BERSERK_COND_ERROR(mUniformSetRHI.isNotNull(), "Failed to update shader uniform bindings on GPU");

            mIsDirtyTextures = mIsDirtyUniformBlocks = false;
        }

        void ShaderUniformBindings::updateDataGPU() {
            if (mIsUniformBuffersDataDirty) {
                for (auto& block: mUniformBlocks) {
                    block.buffer->updateDataGPU();
                }

                mIsUniformBuffersDataDirty = false;
            }
        }

        void ShaderUniformBindings::updateGPU() {
            updateDataGPU();
            updateSetGPU();
        }

        void ShaderUniformBindings::bind(Berserk::RHIDrawList &drawList) {
            BERSERK_COND_ERROR_RET(mUniformSetRHI.isNotNull(), "Null RHI resource");
            drawList.bindUniformSet(mUniformSetRHI);
        }

        void ShaderUniformBindings::showDebugInfo() const {
            mMetaData->showDebugInfo();
        }

    }
}