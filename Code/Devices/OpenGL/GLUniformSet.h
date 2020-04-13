/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLUNIFORMSET_H
#define BERSERK_GLUNIFORMSET_H

#include <RHI/RHIResources.h>
#include <GLUniformBuffer.h>
#include <GLDefinitions.h>
#include <GLSampler.h>
#include <GLTexture.h>
#include <GLErrors.h>
#include <GLShader.h>

namespace Berserk {

    class GLUniformSet : public RHIUniformSet {
    public:

        ~GLUniformSet() override = default;

        bool create(const TArray<RHIUniformTextureDesc> &textures, const TArray<RHIUniformBlockDesc> &uniformBlocks) {
            mTextures = textures;
            mUniformBlocks = uniformBlocks;

            bool foundNull = false;
            uint32 index;

            index = 0;
            for (auto& desc: mTextures) {
                if (desc.sampler.isNull()) {
                    BERSERK_ERROR("Desc sampler is null at [%u] index", index);
                    foundNull = true;
                }
                if (desc.texture.isNull()) {
                    BERSERK_ERROR("Desc texture is null at [%u] index", index);
                    foundNull = true;
                }

                index += 1;
            }

            index = 0;
            for (auto& desc: mUniformBlocks) {
                if (desc.buffer.isNull()) {
                    BERSERK_ERROR("Desc buffer is null at [%u] index", index);
                    foundNull = true;
                }

                index += 1;
            }

            BERSERK_COND_ERROR_RET_VALUE(false, !foundNull, "Incomplete uniform set");

            return true;
        }

        void bind(const GLShader& shader) const {
            uint32 slot = 0;

            for (const auto& textureDesc: mTextures) {
                auto& texture = (GLTexture&)*(textureDesc.texture);
                auto& sampler = (GLSampler&)*(textureDesc.sampler);
                auto location = textureDesc.location;

                texture.bind(location, slot);
                sampler.bind(slot);

                slot += 1;
            }

            for (const auto& uniformBufferDesc: mUniformBlocks) {
                auto& buffer = (GLUniformBuffer&)*(uniformBufferDesc.buffer);
                auto range = uniformBufferDesc.range;
                auto offset = uniformBufferDesc.offset;
                auto binding = uniformBufferDesc.binding;

                buffer.bind(binding, range, offset);
                shader.bindUniformBlock(binding);
            }
        }

    };

}

#endif //BERSERK_GLUNIFORMSET_H