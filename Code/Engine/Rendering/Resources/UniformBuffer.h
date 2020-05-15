/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_UNIFORMBUFFER_H
#define BERSERK_UNIFORMBUFFER_H

#include <Rendering/Resources/IRenderResource.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIShaderMetaData.h>
#include <Math/TVecN.h>
#include <Math/TMatMxN.h>

namespace Berserk {
    namespace Rendering {

        /**
         * @brief Uniform Buffer packer
         *
         * Generic uniform block interface for type-safe CPU -> GPU data transfer.
         * Uses type notation accordingly to the glsl type convention.
         *
         * Extend this class in order to create specific uniform blocks
         * (remember to cache members names to prevent frequent string creation)
         */
        class UniformBuffer : public IRenderResource {
        public:

            using Vec2 = TVecN<float,2>;
            using Vec3 = TVecN<float,3>;
            using Vec4 = TVecN<float,4>;
            using Mat2 = TMatMxN<float,2,2>;
            using Mat3 = TMatMxN<float,3,3>;
            using Mat4 = TMatMxN<float,4,4>;

            /**
             * Creates uniform buffer for specified shader uniform block structure.
             * @note Uniform block of the same structure could be used among several shaders
             * @note This UniformBuffer stores ref to the shader block, therefore shader must stay valid
             * @param name Friendly name for debug
             * @param block Shader block reference
             */
            UniformBuffer(CString name, const ShaderUniformBlock& block);
            ~UniformBuffer() override;

            bool isInitialized() const override;
            bool isInitializedRHI() const override;
            CString getFriendlyName() const override;

            /**
             * Set float param to the uniform buffer (not for arrays)
             * @param name Name of the member in the uniform block (in the shader)
             * @param value Value to set (caches writes on the CPU)
             * @return True if member found an successfully updated
             */
            bool setFloat(const CString& name, float value);

            /**
             * Set vec2 param to the uniform buffer (not for arrays)
             * @param name Name of the member in the uniform block (in the shader)
             * @param value Value to set (caches writes on the CPU)
             * @return True if member found an successfully updated
             */
            bool setVec2(const CString& name, const Vec2 &value);

            /**
             * Set vec3 param to the uniform buffer (not for arrays)
             * @param name Name of the member in the uniform block (in the shader)
             * @param value Value to set (caches writes on the CPU)
             * @return True if member found an successfully updated
             */
            bool setVec3(const CString& name, const Vec3 &value);

            /**
             * Set vec4 param to the uniform buffer (not for arrays)
             * @param name Name of the member in the uniform block (in the shader)
             * @param value Value to set (caches writes on the CPU)
             * @return True if member found an successfully updated
             */
            bool setVec4(const CString& name, const Vec4 &value);

            /**
             * Set mat2 param to the uniform buffer (not for arrays)
             * @param name Name of the member in the uniform block (in the shader)
             * @param value Value to set (caches writes on the CPU)
             * @return True if member found an successfully updated
             */
            bool setMat2(const CString& name, const Mat2 &value);

            /**
             * Set mat3 param to the uniform buffer (not for arrays)
             * @param name Name of the member in the uniform block (in the shader)
             * @param value Value to set (caches writes on the CPU)
             * @return True if member found an successfully updated
             */
            bool setMat3(const CString& name, const Mat3 &value);

            /**
             * Set mat4 param to the uniform buffer (not for arrays)
             * @param name Name of the member in the uniform block (in the shader)
             * @param value Value to set (caches writes on the CPU)
             * @return True if member found an successfully updated
             */
            bool setMat4(const CString& name, const Mat4 &value);

            /** Updates data on GPU */
            void updateUniformBufferDataGPU();

            /** @return RHI uniform buffer resource */
            const TPtrShared<RHIUniformBuffer> &getUniformBufferRHI() const { return mUniformBuffer; }

        protected:
            CString mFriendlyName = "UniformBuffer";
            TRef<const ShaderUniformBlock> mUniformBlockLayout;
            TPtrShared<RHIUniformBuffer> mUniformBuffer;
            TArray<uint8> mCachedUniformData;
        };


    }
}

#endif //BERSERK_UNIFORMBUFFER_H