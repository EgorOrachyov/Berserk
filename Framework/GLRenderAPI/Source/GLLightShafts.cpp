//
// Created by Egor Orachyov on 26.08.2018.
//

#include "Pipeline/GLLightShafts.h"
#include "Render/GLRenderDriver.h"
#include "Render/GLRenderSystem.h"
#include "Math/UtilityMatrices.h"

namespace Berserk
{

    void GLLightShafts::init()
    {
        mScreenMap.init();
        mScreenMap.compileShader("../GLRenderAPI/Shaders/PostProcess/LightShafts/GLSLScreenMap.vert", GLShaderType::GLST_VERTEX);
        mScreenMap.compileShader("../GLRenderAPI/Shaders/PostProcess/LightShafts/GLSLScreenMap.frag", GLShaderType::GLST_FRAGMENT);
        mScreenMap.link();
        mScreenMap.validate();

        mUniform.Depth = mScreenMap.getUniformLocation("Depth");
        mUniform.LightColor = mScreenMap.getUniformLocation("LightColor");
        mUniform.LightBackColor = mScreenMap.getUniformLocation("LightBackColor");
        mUniform.LightPosition = mScreenMap.getUniformLocation("LightPosition");
        mUniform.LightRadius = mScreenMap.getUniformLocation("LightRadius");

        mRadialBlur.init();
        mRadialBlur.compileShader("../GLRenderAPI/Shaders/PostProcess/LightShafts/GLSLRadialBlur.vert", GLShaderType::GLST_VERTEX);
        mRadialBlur.compileShader("../GLRenderAPI/Shaders/PostProcess/LightShafts/GLSLRadialBlur.frag", GLShaderType::GLST_FRAGMENT);
        mRadialBlur.link();
        mRadialBlur.validate();

        mUniform.Exposure = mRadialBlur.getUniformLocation("Exposure");
        mUniform.Decay = mRadialBlur.getUniformLocation("Decay");
        mUniform.Density = mRadialBlur.getUniformLocation("Density");
        mUniform.Weight = mRadialBlur.getUniformLocation("Weight");
        mUniform.Position = mRadialBlur.getUniformLocation("Position");
        mUniform.LightMap = mRadialBlur.getUniformLocation("LightMap");

        mBlurBlend.init();
        mBlurBlend.compileShader("../GLRenderAPI/Shaders/PostProcess/LightShafts/GLSLBlurBlend.vert", GLShaderType::GLST_VERTEX);
        mBlurBlend.compileShader("../GLRenderAPI/Shaders/PostProcess/LightShafts/GLSLBlurBlend.frag", GLShaderType::GLST_FRAGMENT);
        mBlurBlend.link();
        mBlurBlend.validate();

        mUniform.Input = mBlurBlend.getUniformLocation("Input");
        mUniform.Blend = mBlurBlend.getUniformLocation("Blend");

        auto ScreenPart = gRenderSystem->getLightShaftsBufferSize();
        mWidth = (UINT32)(ScreenPart * gRenderSystem->getPixelWindowWidth());
        mHeight = (UINT32)(ScreenPart * gRenderSystem->getPixelWindowHeight());
        mBuffer1.create(mWidth, mHeight);
        mBuffer2.create(mWidth, mHeight);
    }

    void GLLightShafts::destroy()
    {
        mScreenMap.destroy();
        mRadialBlur.destroy();
        mBlurBlend.destroy();

        mBuffer1.destroy();
        mBuffer2.destroy();
    }

    void GLLightShafts::execute()
    {
        auto render = dynamic_cast<GLRenderSystem*>(gRenderSystem);
        auto driver = dynamic_cast<GLRenderDriver*>(gRenderDriver);

        if (!render->getGlobalLight() || !render->getGlobalLight()->castLightShafts())
        {
            return;
        }

        Camera* camera = render->getRenderCamera();
        const CameraComponent* component = camera->getComponent();
        const GlobalLight* light = render->getGlobalLight();
        Vector3f position = projectOnScreen(component->mProjection * component->mView, light->getWorldPosition());

        if (render->isReSized())
        {
            auto ScreenPart = gRenderSystem->getLightShaftsBufferSize();

            mWidth = (UINT32)(ScreenPart * gRenderSystem->getPixelWindowWidth());
            mHeight = (UINT32)(ScreenPart * gRenderSystem->getPixelWindowHeight());

            mBuffer1.destroy();
            mBuffer1.create(mWidth, mHeight);

            mBuffer2.destroy();
            mBuffer2.create(mWidth, mHeight);
        }

        // Gen light map

        mBuffer1.useAsFBO();

        driver->enableDepthTest(false);
        driver->setViewPort(0, 0, mWidth, mHeight);
        driver->enableFaceCulling(true);
        driver->setBackCulling();
        driver->setWindingOrderCCW();

        mScreenMap.use();
        mScreenMap.setUniform(mUniform.Depth, 0);
        mScreenMap.setUniform(mUniform.LightColor, light->getSphereColor());
        mScreenMap.setUniform(mUniform.LightBackColor, light->getBackColor());
        mScreenMap.setUniform(mUniform.LightRadius, light->getSphereRadius());
        mScreenMap.setUniform(mUniform.LightPosition, position);
        mScreenMap.setUniform("Upscale", camera->getRatioOfViewBorders()); // todo : remove

        render->getGBuffer()->useAsUniformDepthBuffer(0);
        render->getScreenPlane()->use();

        // Gen radial blur

        auto weight = (FLOAT32)clamp(
                -Vector3f::dot(
                Vector3f::normalize(camera->getWorldDirection()),
                Vector3f::normalize(light->getWorldDirection())), 0.0, 1.0
        );

        mBuffer2.useAsFBO();
        mRadialBlur.use();
        mRadialBlur.setUniform(mUniform.Weight, 0.1f * weight);
        mRadialBlur.setUniform(mUniform.Exposure, render->getLightShaftsExposure());
        mRadialBlur.setUniform(mUniform.Decay, render->getLightShaftsDecay());
        mRadialBlur.setUniform(mUniform.Density, 1.0f);
        mRadialBlur.setUniform(mUniform.LightMap, 0);
        mRadialBlur.setUniform(mUniform.Position, Vector2f(position));
        mBuffer1.useAsUniform(0);
        render->getScreenPlane()->use();

        // Blur light shafts

        render->getStageOutBuffer()->useAsFBO();
        driver->setViewPort(0,0,render->getPixelWindowWidth(), render->getPixelWindowHeight());
        mBlurBlend.use();
        mBlurBlend.setUniform(mUniform.Blend, 0);
        mBlurBlend.setUniform(mUniform.Input, 1);
        mBuffer2.useAsUniform(1);
        render->getStageInBuffer()->useAsUniformData();
        render->getScreenPlane()->use();
    }

} // namespace Berserk