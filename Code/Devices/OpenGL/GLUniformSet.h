/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
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

        void create(const TArray<RHIUniformTextureDesc> &textures, const TArray<RHIUniformBlockDesc> &uniformBlocks) {
            mTextures = textures;
            mUniformBlocks = uniformBlocks;
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