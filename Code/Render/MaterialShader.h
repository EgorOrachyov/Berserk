/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MATERIALSHADER_H
#define BERSERK_MATERIALSHADER_H

#include <Shader.h>
#include <Material.h>
#include <RenderResources/UniformBuffer.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Material shader
         *
         * Represents a complete GPU state and material data, required for the
         * rendering of the scene geometry. This material shader provides
         * graphics pipeline state and material uniform data.
         *
         * Material shaders compiled time for each vertex declaration type of the
         * mesh geometry for each material feature flags and shading techniques.
         *
         * Material shaders provides sorting feature, accordingly to the
         * vertex declaration structure and used material.
         */
        class MaterialShader final: public Shader, public MaterialListener {
        public:

            /**
             * Creates shader material from compiled program and loaded vertex declaration
             * @param name Shader name
             * @param material Material reference, this shader is created for
             * @param program Compiled program
             * @param declaration Vertex declaration layout
             */
            MaterialShader(CString name, Material& material, TPtrShared<ShaderProgram> program, TPtrShared<VertexDeclaration> declaration);
            ~MaterialShader() override;

            /** Used to sort material shader before rendering */
            bool operator<=(const MaterialShader& other);

            /** Update uniform buffer data (from MaterialListener) */
            void notifyOnChanged() override;

            /** @return Uniform block reference meta info */
            TRef<const ShaderUniformBlock> getBlockTransformData() const { return pTransformData; };

            /** @return Uniform block reference meta info */
            TRef<const ShaderUniformBlock> getBlockCameraData() const { return pCameraData; }

            /** @return Uniform block reference meta info */
            TRef<const ShaderUniformBlock> getBlockSceneLights() const { return pSceneLights; }

            /** @return Uniform block reference meta info */
            TRef<const ShaderUniformBlock> getBlockAffectingLights() const { return pAffectingLights; }

            /** @return Uniform block reference meta info */
            TRef<const ShaderUniformBlock> getBlockMaterialData() const { return pMaterialData; }

            /** @return Material uniform block data  */
            const UniformBuffer& getMaterialData() const { return mMaterialData; }

            /** @return Pipeline state for rendering */
            const RHIGraphicsPipelineState& getPipelineState() const { return mPipelineState; }

        private:
            using BlockRef = TRef<const ShaderUniformBlock>;
            using MemberRef = TRef<const ShaderBlockMember>;
            using ParamRef = TRef<const ShaderParam>;

            RHIGraphicsPipelineState mPipelineState;
            UniformBuffer mMaterialData;
            Material& mMaterial;

            BlockRef pTransformData;
            BlockRef pCameraData;
            BlockRef pSceneLights;
            BlockRef pAffectingLights;
            BlockRef pMaterialData;

            MemberRef pAlbedoColor;
            MemberRef pEmissionColor;
            MemberRef pAlpha;
            MemberRef pSpecular;
            MemberRef pMetallic;
            MemberRef pRoughness;
            MemberRef pAmbientScale;
            MemberRef pInverseGamma;
            MemberRef pIsAlbedoSRGB;

            ParamRef pTextureAlbedo;
            ParamRef pTextureSpecular;
            ParamRef pTextureMetallic;
            ParamRef pTextureRoughness;
            ParamRef pTextureNormal;
            ParamRef pTextureDisplacement;
            ParamRef pTextureAmbient;
            ParamRef pTextureEmission;
        };


    }
}



#endif //BERSERK_MATERIALSHADER_H
