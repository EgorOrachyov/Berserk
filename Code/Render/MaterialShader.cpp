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

        MaterialShader::MaterialShader(CString name, Material &material, TPtrShared<ShaderProgram> program, TPtrShared<VertexDeclaration> declaration)
            : Shader(std::move(name), std::move(program), std::move(declaration)),
              mMaterial(material) {

            auto& meta = getShaderMetaRHI();

            pTransformData = meta->getUniformBlock("TransformData");
            pCameraData = meta->getUniformBlock("CameraData");
            pSceneLights = meta->getUniformBlock("SceneLights");
            pAffectingLights = meta->getUniformBlock("AffectingLights");
            pMaterialData = meta->getUniformBlock("MaterialData");

            pAlbedoColor = pMaterialData->getMember("albedoColor");
            pEmissionColor = pMaterialData->getMember("emissionColor");;
            pAlpha = pMaterialData->getMember("alpha");
            pSpecular = pMaterialData->getMember("specular");
            pMetallic = pMaterialData->getMember("metallic");
            pRoughness = pMaterialData->getMember("roughness");
            pAmbientScale = pMaterialData->getMember("ambientScale");
            pInverseGamma = pMaterialData->getMember("inverseGamma");
            pIsAlbedoSRGB = pMaterialData->getMember("isAlbedoSRGB");

            pTextureAlbedo = meta->getParam("TextureAlbedo");
            pTextureSpecular = meta->getParam("TextureSpecular");
            pTextureMetallic = meta->getParam("TextureMetallic");
            pTextureRoughness = meta->getParam("TextureRoughness");
            pTextureNormal = meta->getParam("TextureNormal");
            pTextureDisplacement = meta->getParam("TextureDisplacement");
            pTextureAmbient = meta->getParam("TextureAmbient");
            pTextureEmission = meta->getParam("TextureEmission");
            
            mMaterial.subscribe(*this);

            // Fill the uniform data in first time
            notifyOnChanged();
        }

        MaterialShader::~MaterialShader() {
            mMaterial.unsubscribe(*this);
        }

        bool MaterialShader::operator<=(const MaterialShader &other) {
            return true;
        }

        void MaterialShader::notifyOnChanged() {
            // Must update uniform data
            // .... .... ....
        }

    }
}