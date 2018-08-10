//
// Created by Egor Orachyov on 29.07.2018.
//

#include "Pipeline/GLFragmentLightning.h"

namespace Berserk
{

    GLFragmentLightning::GLFragmentLightning()
    {

    }

    GLFragmentLightning::~GLFragmentLightning()
    {
        destroy();
    }

    void GLFragmentLightning::init()
    {
        mProgram.init();
        mProgram.compileShader("../GLRenderAPI/Shaders/MainProcess/GLSLFragmentLightning.vert", GLShaderType::GLST_VERTEX);
        mProgram.compileShader("../GLRenderAPI/Shaders/MainProcess/GLSLFragmentLightning.frag", GLShaderType::GLST_FRAGMENT);
        mProgram.link();
        mProgram.validate();

        mUniform.ModelView = mProgram.getUniformLocation("ModelView");
        mUniform.MVP = mProgram.getUniformLocation("MVP");

        mUniform.Material.Ambient = mProgram.getUniformLocation("Material.Ambient");
        mUniform.Material.Diffuse = mProgram.getUniformLocation("Material.Diffuse");
        mUniform.Material.Specular = mProgram.getUniformLocation("Material.Specular");
        mUniform.Material.Shininess = mProgram.getUniformLocation("Material.Shininess");

        mUniform.ambientLight = mProgram.getUniformLocation("ambientLight");

        CHAR buffer[BUFFER_SIZE_64];

        for(UINT32 i = 0; i < LightInfo::LI_MAX_DIRECTIONAL_LIGHTS; i++)
        {
            sprintf(buffer, "directionalLights[%u].Direction", i);
            mUniform.directLights[i].Direction = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "directionalLights[%u].Intensity", i);
            mUniform.directLights[i].Intensity = mProgram.getUniformLocation(buffer);
        }

        for(UINT32 i = 0; i < LightInfo::LI_MAX_POINT_LIGHTS; i++)
        {
            sprintf(buffer, "pointLights[%u].Position", i);
            mUniform.pointLights[i].Position = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointLights[%u].Intensity", i);
            mUniform.pointLights[i].Intensity = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointLights[%u].constant", i);
            mUniform.pointLights[i].constant = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointLights[%u].linear", i);
            mUniform.pointLights[i].linear = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointLights[%u].quadratic", i);
            mUniform.pointLights[i].quadratic = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointLights[%u].radius", i);
            mUniform.pointLights[i].radius = mProgram.getUniformLocation(buffer);
        }

        for(UINT32 i = 0; i < LightInfo::LI_MAX_SPOT_LIGHTS; i++)
        {
            sprintf(buffer, "spotLights[%u].Position", i);
            mUniform.spotLights[i].Position = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLights[%u].Direction", i);
            mUniform.spotLights[i].Direction = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLights[%u].Intensity", i);
            mUniform.spotLights[i].Intensity = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLights[%u].cutoff", i);
            mUniform.spotLights[i].cutoff = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLights[%u].outerCutoff", i);
            mUniform.spotLights[i].outerCutoff = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLights[%u].epsilon", i);
            mUniform.spotLights[i].epsilon = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLights[%u].exponent", i);
            mUniform.spotLights[i].exponent = mProgram.getUniformLocation(buffer);
        }

        mUniform.NUM_OF_DIR_LIGHTS = mProgram.getUniformLocation("NUM_OF_DIR_LIGHTS");
        mUniform.NUM_OF_SPOT_LIGHTS = mProgram.getUniformLocation("NUM_OF_SPOT_LIGHTS");
        mUniform.NUM_OF_POINT_LIGHTS = mProgram.getUniformLocation("NUM_OF_POINT_LIGHTS");
    }

    void GLFragmentLightning::destroy()
    {
        mProgram.destroy();
    }

    GLFrameBufferObject* GLFragmentLightning::process(RenderManager *manager, GLFrameBufferObject *object)
    {
        if (manager->getCamera() == NULL)
        {
            WARNING("No camera component in rendering queue");
            return NULL;
        }

        glViewport(manager->getCamera()->mViewport.posX,
                   manager->getCamera()->mViewport.posY,
                   manager->getCamera()->mViewport.width,
                   manager->getCamera()->mViewport.height);

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        mProgram.use();
        const Matrix4x4f &View = manager->getCamera()->mView;

        // Uniform number of active lights
        {
            mProgram.setUniform(mUniform.NUM_OF_DIR_LIGHTS, manager->getDirectionalLights().getSize());
            mProgram.setUniform(mUniform.NUM_OF_SPOT_LIGHTS, manager->getSpotLights().getSize());
            mProgram.setUniform(mUniform.NUM_OF_POINT_LIGHTS, manager->getPointLights().getSize());
        }

        // Spot Lights Uniform Block
        {
            const List<SpotLightComponent *> &spotLights = manager->getSpotLights();
            for (UINT32 i = 0; i < spotLights.getSize(); i++)
            {
                SpotLightComponent *spotLightComponent = spotLights.get(i);
                mProgram.setUniform(mUniform.spotLights[i].Position, View * spotLightComponent->mPosition);
                mProgram.setUniform(mUniform.spotLights[i].Direction, View * spotLightComponent->mDirection);
                mProgram.setUniform(mUniform.spotLights[i].Intensity, spotLightComponent->mLightIntensity);
                mProgram.setUniform(mUniform.spotLights[i].cutoff, spotLightComponent->mCutoff);
                mProgram.setUniform(mUniform.spotLights[i].outerCutoff, spotLightComponent->mOuterCutoff);
                mProgram.setUniform(mUniform.spotLights[i].epsilon, spotLightComponent->mEpsilon);
                mProgram.setUniform(mUniform.spotLights[i].exponent, spotLightComponent->mAttenuationExponent);
            }
        }

        // Point Lights Uniform Block
        {
            const List<PointLightComponent*> &pointLights = manager->getPointLights();
            for(UINT32 i = 0; i < pointLights.getSize(); i++)
            {
                PointLightComponent *pointLightComponent = pointLights.get(i);
                mProgram.setUniform(mUniform.pointLights[i].Position, View * pointLightComponent->mPosition);
                mProgram.setUniform(mUniform.pointLights[i].Intensity, pointLightComponent->mLightIntensity);
                mProgram.setUniform(mUniform.pointLights[i].constant, pointLightComponent->mConstantAttenuation);
                mProgram.setUniform(mUniform.pointLights[i].linear, pointLightComponent->mLinearAttenuation);
                mProgram.setUniform(mUniform.pointLights[i].quadratic, pointLightComponent->mQuadraticAttenuation);
                mProgram.setUniform(mUniform.pointLights[i].radius, pointLightComponent->mRadius);
            }
        }

        // Directional Lights Uniform Block
        {
            const List<DirectionalLightComponent*> &directLights = manager->getDirectionalLights();
            for(UINT32 i = 0; i < directLights.getSize(); i++)
            {
                DirectionalLightComponent *directionalLightComponent = directLights.get(i);
                mProgram.setUniform(mUniform.directLights[i].Direction, View * directionalLightComponent->mDirection);
                mProgram.setUniform(mUniform.directLights[i].Intensity, directionalLightComponent->mLightIntensity);
            }
        }

        // Ambient Light uniform
        {
            if (manager->getAmbientLight() != NULL)
                mProgram.setUniform(mUniform.ambientLight, manager->getAmbientLight()->mLightIntensity);
        }

        // Base Material Rendering Part
        {
            const List<BaseRenderMaterialComponent> &baseRenderMaterial = manager->getBaseRenderMaterials();
            for(UINT32 i = 0; i < baseRenderMaterial.getSize(); i++)
            {
                BaseRenderMaterialComponent &baseRenderMaterialComponent = baseRenderMaterial.get(i);
                mProgram.setUniform(mUniform.Material.Ambient, baseRenderMaterialComponent.mMaterialComponent->mAmbient);
                mProgram.setUniform(mUniform.Material.Diffuse, baseRenderMaterialComponent.mMaterialComponent->mDiffuse);
                mProgram.setUniform(mUniform.Material.Specular, baseRenderMaterialComponent.mMaterialComponent->mSpecular);
                mProgram.setUniform(mUniform.Material.Shininess, baseRenderMaterialComponent.mMaterialComponent->mShininess);

                Matrix4x4f ModelView = manager->getCamera()->mView * (*baseRenderMaterialComponent.mTransformationComponent);
                Matrix4x4f MVP = manager->getCamera()->mProjection * ModelView;

                mProgram.setUniform(mUniform.ModelView, ModelView);
                mProgram.setUniform(mUniform.MVP, MVP);

                baseRenderMaterialComponent.mRenderBufferComponent->drawIndices();
            }
        }

        return NULL;
    }

}