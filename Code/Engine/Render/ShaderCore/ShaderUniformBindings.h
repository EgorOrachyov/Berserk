/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERUNIFORMBINDINGS_H
#define BERSERK_SHADERUNIFORMBINDINGS_H

#include <RHI/RHIDescs.h>
#include <RHI/RHIUniformSet.h>
#include <RHI/RHIShaderMetaData.h>
#include <Containers/TMap.h>
#include <Containers/TArray.h>
#include <RenderResources/Texture.h>
#include <RenderResources/UniformBuffer.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Shader binding
         *
         * Stores all the shader uniform bindings, required for rendering on the GPU.
         *
         * Provides query for params meta structure, required for fast and safe params setup.
         * Also provides set via string names of the variables and textures.
         */
        class ShaderUniformBindings {
        public:

            using Vec2 = TVecN<float,2>;
            using Vec3 = TVecN<float,3>;
            using Vec4 = TVecN<float,4>;
            using Mat2 = TMatMxN<float,2,2>;
            using Mat3 = TMatMxN<float,3,3>;
            using Mat4 = TMatMxN<float,4,4>;

            class TextureParam {
                friend class ShaderUniformBindings;

                const ShaderParam* param = nullptr;
                int32 index = -1;
            };

            class BlockParam {
                friend class ShaderUniformBindings;

                const ShaderMember* param = nullptr;
                int32 index = -1;
            };

            /** Creates uniform bindings for specified layout */
            ShaderUniformBindings(TPtrShared<RHIShaderMetaData> metaData);

            /** Associate uniform buffer region with offset for specified block name (range size calculated from block size) */
            void associateUniformBuffer(const CString& blockName, uint32 offset, TPtrShared<UniformBuffer> buffer);

            /** Associate uniform buffer for specified block name */
            void associateUniformBuffer(const CString& blockName, TPtrShared<UniformBuffer> buffer);

            /** Associate new uniform buffer for specified block name */
            void associateUniformBuffer(const CString& blockName);

            /** Creates new empty uniform buffers for every uniform block, which currently has no associated uniform buffer */
            void associateUniformBuffers();

            /** Fast set texture value via param meta info */
            void setTexture2D(const TextureParam &param, const TPtrShared<Texture> &t);

            /** Fast set float value via param meta info */
            void setFloat(const BlockParam& param, float t);

            /** Fast set vec2 value via param meta info */
            void setVec2(const BlockParam& param, const Vec2& t);

            /** Fast set vec3 value via param meta info */
            void setVec3(const BlockParam& param, const Vec3& t);

            /** Fast set vec4 value via param meta info */
            void setVec4(const BlockParam& param, const Vec4& t);

            /** Fast set mat2 value via param meta info */
            void setMat2(const BlockParam& param, const Mat2& t);

            /** Fast set mat3 value via param meta info */
            void setMat3(const BlockParam& param, const Mat3& t);

            /** Fast set mat4 value via param meta info */
            void setMat4(const BlockParam& param, const Mat4& t);

            /**
             * Finds texture meta info for specified texture name
             * @param texture Name of the texture
             * @return texture meta info
             */
            TextureParam findTexture2D(const CString &texture) const;

            /**
             * Finds block param meta info for specified uniform block and param
             * @param uniformBlock Name of the uniform block where param is actually stored
             * @param paramName Name of the param in the uniform block
             * @return param meta info
             */
            BlockParam findParam(const CString& uniformBlock, const CString& paramName) const;

            /** @return True if requires update on GPU */
            bool isDirty() const;

            /** Updates RHI uniform set on GPU */
            void updateDataGPU();

            /** @return RHI uniform set resource */
            const TPtrShared<RHIUniformSet> &getRHI() const { return mUniformSetRHI; }

            /** Output debug info */
            void showDebugInfo() const;

        private:

            struct TextureBinding {
                TPtrShared<Texture> texture;
                int32 location;
            };

            struct UniformBlockBinding {
                TPtrShared<UniformBuffer> buffer;
                uint32 offset;
                uint32 range;
                int32 binding;
            };

            TArray<TextureBinding> mTextures;
            TArray<UniformBlockBinding> mUniformBlocks;

            /** Actual RHI resource */
            TPtrShared<RHIUniformSet> mUniformSetRHI;
            TPtrShared<RHIShaderMetaData> mMetaData;

            bool mIsUniformBuffersDataDirty = true;
            bool mIsDirtyTextures = true;
            bool mIsDirtyUniformBlocks = true;
        };

    }
}

#endif //BERSERK_SHADERUNIFORMBINDINGS_H