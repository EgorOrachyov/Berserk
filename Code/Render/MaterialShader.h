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
         * Represents a compiled RHI shader for material geometry rendering.
         */
        class MaterialShader final: public Shader {
        public:

            /**
             * Creates shader material from compiled program and loaded vertex declaration
             * @param name Shader name
             * @param material Material reference, this shader is created for
             * @param program Compiled program
             * @param declaration Vertex declaration layout
             */
            MaterialShader(CString name, TPtrShared<ShaderProgram> program, TPtrShared<VertexDeclaration> declaration);
            ~MaterialShader() override = default;

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

            /** @return Uniform textures reference meta info */
            TArrayStatic<TRef<const ShaderParam>,Material::MAX_TEXTURES> getTextures() const { return pTextures; };

        private:
            using BlockRef = TRef<const ShaderUniformBlock>;
            using ParamRef = TRef<const ShaderParam>;

            BlockRef pTransformData;
            BlockRef pCameraData;
            BlockRef pSceneLights;
            BlockRef pAffectingLights;
            BlockRef pMaterialData;

            TArrayStatic<ParamRef,Material::MAX_TEXTURES> pTextures;
        };
    }
}



#endif //BERSERK_MATERIALSHADER_H
