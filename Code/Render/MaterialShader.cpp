/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <MaterialShader.h>

namespace Berserk {
    namespace Render {

        MaterialShader::MaterialShader(CString name, TPtrShared<ShaderProgram> program, TPtrShared<VertexDeclaration> declaration)
            : Shader(std::move(name), std::move(program), std::move(declaration)) {

            auto& meta = getShaderMetaRHI();

            pTransformData = meta->getUniformBlock("TransformData");
            pCameraData = meta->getUniformBlock("CameraData");
            pSceneLights = meta->getUniformBlock("SceneLights");
            pAffectingLights = meta->getUniformBlock("AffectingLights");
            pMaterialData = meta->getUniformBlock("MaterialData");

            const char* texturesNames[] = {
                "TextureAlbedo",
                "TextureSpecular",
                "TextureMetallic",
                "TextureRoughness",
                "TextureNormal",
                "TextureDisplacement",
                "TextureAmbient",
                "TextureEmission"
            };

            for (auto textureName: texturesNames) {
                pTextures.add(meta->getParam(textureName));
            }
        }

    }
}