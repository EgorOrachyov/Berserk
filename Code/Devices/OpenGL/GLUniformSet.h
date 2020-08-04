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

        bool create(const RHIUniformTextures &textures, const RHIUniformBlocks &uniformBlocks) {
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

        static void setSetAlloc(AllocPool& setAlloc) {
            gSetAlloc = &setAlloc;
        }

        static AllocPool& getSetAlloc() {
            return *gSetAlloc;
        }

        static TPtrShared<GLUniformSet> createStatic(const RHIUniformTextures &textures, const RHIUniformBlocks &uniformBlocks) {
            static auto dealloc = [](void* mem){ GLUniformSet::getSetAlloc().free(mem); };
            static Function<void(void*)> func = dealloc;

            void* memory = gSetAlloc->allocate(sizeof(GLUniformSet));
            auto set = new (memory) GLUniformSet();
            auto result = set->create(textures, uniformBlocks);

            if (!result) {
                // Release set and return memory to pool
                set->~GLUniformSet();
                gSetAlloc->free(memory);
                return nullptr;
            }

            return TPtrShared<GLUniformSet>(set, &func);
        }

    private:

        /** Allocate set from here */
        static AllocPool* gSetAlloc;

    };

}

#endif //BERSERK_GLUNIFORMSET_H