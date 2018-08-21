//
// Created by Egor Orachyov on 21.08.2018.
//

#include "Pipeline/GLAmbientOcclusion.h"
#include "Render/GLRenderDriver.h"
#include "Render/GLRenderSystem.h"
#include "Math/UtilityNumbers.h"

namespace Berserk
{

    void GLAmbientOcclusion::init()
    {
        mSSAO.init();
        mSSAO.compileShader("../GLRenderAPI/Shaders/PostProcess/SSAO/GLSLAmbientOcclusion.vert", GLShaderType::GLST_VERTEX);
        mSSAO.compileShader("../GLRenderAPI/Shaders/PostProcess/SSAO/GLSLAmbientOcclusion.frag", GLShaderType::GLST_FRAGMENT);
        mSSAO.link();
        mSSAO.validate();

        mUniform.gPosition = mSSAO.getUniformLocation("gPosition");
        mUniform.gNormal = mSSAO.getUniformLocation("gNormal");
        mUniform.Noise = mSSAO.getUniformLocation("Noise");

        mUniform.samplesCount = mSSAO.getUniformLocation("samplesCount");
        mUniform.noiseScale = mSSAO.getUniformLocation("noiseScale");
        mUniform.radius = mSSAO.getUniformLocation("radius");

        mUniform.View = mSSAO.getUniformLocation("View");
        mUniform.Proj = mSSAO.getUniformLocation("Proj");
        mUniform.PV = mSSAO.getUniformLocation("PV");

        for (UINT32 i = 0; i < SSAOInfo::SSAO_KERNEL_SIZE; ++i)
        {
            CHAR buffer[20];
            sprintf(buffer, "samples[%u]", i);
            mUniform.samples[i] = mSSAO.getUniformLocation(buffer);
        }

        const UINT32 NOISE_SIZE = SSAOInfo::SSAO_NOISE_SIZE;
        mNoise.create(NOISE_SIZE);
        mNoiseFactor = (FLOAT32)1.0 / (FLOAT32)NOISE_SIZE;
        mNoiseScale = Vector2f(mNoiseFactor * (FLOAT32)gRenderSystem->getPixelWindowWidth(),
                               mNoiseFactor * (FLOAT32)gRenderSystem->getPixelWindowHeight());

        mRadius = 0.8;
        mPartOfScreenSize = 0.5;
        mWidth = (UINT32)(mPartOfScreenSize * gRenderSystem->getPixelWindowWidth());
        mHeight = (UINT32)(mPartOfScreenSize * gRenderSystem->getPixelWindowHeight());

        mBuffer.create(mWidth, mHeight);

        for (UINT32 i = 0; i < SSAOInfo::SSAO_KERNEL_SIZE; ++i)
        {
            mKernel[i] = Vector3f(random(-1.0f, 1.0f),
                                  random(-1.0f, 1.0f),
                                  random(0.0f, 1.0f));

            mKernel[i].normalize();

            FLOAT32 part = (FLOAT32)i / (FLOAT32)SSAOInfo::SSAO_KERNEL_SIZE;
            FLOAT32 scale = 0.1f + part * part * (1.0f - 0.1f);

            mKernel[i] = mKernel[i] * scale;
        }


        debug.init();
        debug.compileShader("../GLRenderAPI/Shaders/PreProcess/Deferred/Debug.vert", GLShaderType::GLST_VERTEX);
        debug.compileShader("../GLRenderAPI/Shaders/PreProcess/Deferred/Debug.frag", GLShaderType::GLST_FRAGMENT);
        debug.link();
        debug.validate();
    }

    void GLAmbientOcclusion::destroy()
    {
        mSSAO.destroy();
        mBuffer.destroy();
    }

    void GLAmbientOcclusion::execute()
    {
        auto render = dynamic_cast<GLRenderSystem*>(gRenderSystem);
        auto driver = dynamic_cast<GLRenderDriver*>(gRenderDriver);

        if (render->isReSized())
        {
            mNoiseScale = Vector2f(mNoiseFactor * (FLOAT32)gRenderSystem->getPixelWindowWidth(),
                                   mNoiseFactor * (FLOAT32)gRenderSystem->getPixelWindowHeight());

            mWidth = (UINT32)(mPartOfScreenSize * gRenderSystem->getPixelWindowWidth());
            mHeight = (UINT32)(mPartOfScreenSize * gRenderSystem->getPixelWindowHeight());

            mBuffer.destroy();
            mBuffer.create(mWidth, mHeight);
        }

        Matrix4x4f& View = render->getRenderCamera()->getComponent()->mView;
        Matrix4x4f& Proj = render->getRenderCamera()->getComponent()->mProjection;
        Matrix4x4f  PV   = Proj * View;

        GBuffer* gBuffer = render->getGBuffer();

        mBuffer.useAsFBO();

        driver->setClearColor(Vector4f(0.0));
        driver->clearColorBuffer();
        driver->enableDepthTest(false);
        driver->setViewPort(0, 0, mWidth, mHeight);
        driver->enableFaceCulling(true);
        driver->setBackCulling();
        driver->setWindingOrderCCW();

        mSSAO.use();

        mSSAO.setUniform(mUniform.gPosition, 0);
        mSSAO.setUniform(mUniform.gNormal, 1);
        mSSAO.setUniform(mUniform.Noise, 2);

        mSSAO.setUniform(mUniform.samplesCount, (UINT32)SSAOInfo::SSAO_KERNEL_SIZE);
        mSSAO.setUniform(mUniform.noiseScale, mNoiseScale);
        mSSAO.setUniform(mUniform.radius, mRadius);

        for (UINT32 i = 0; i < SSAOInfo::SSAO_KERNEL_SIZE; i++)
        {
            mSSAO.setUniform(mUniform.samples[i], mKernel[i]);
        }

        mSSAO.setUniform(mUniform.View, View);
        mSSAO.setUniform(mUniform.Proj, Proj);
        mSSAO.setUniform(mUniform.PV, PV);

        gBuffer->useAsUniformLayout(GBufferInfo::GBI_POSITION_SLOT, GBufferInfo::GBI_POSITION_SLOT);
        gBuffer->useAsUniformLayout(GBufferInfo::GBI_NORMAL_SLOT, GBufferInfo::GBI_NORMAL_SLOT);
        mNoise.useAsUniform(2);

        render->getScreenPlane()->use();

        const CameraComponent::Viewport& Port = render->getRenderCamera()->getComponent()->mViewport;
        driver->setDefaultBuffer();
        driver->clearBuffer();
        driver->setViewPort(Port.posX, Port.posY, Port.width, Port.height);

        debug.use();
        debug.setUniform("Screen", 0);
        mBuffer.useAsUniform(0);

        render->getScreenPlane()->use();
    }

} //namespace Berserk