//
// Created by Egor Orachyov on 21.08.2018.
//

#include "Pipeline/GLAmbientOcclusion.h"
#include "Render/GLRenderDriver.h"
#include "Render/GLRenderSystem.h"

namespace Berserk
{

    void GLAmbientOcclusion::init()
    {
        mSSAO.init();
        mSSAO.compileShader("../GLRenderAPI/Shaders/PostProcess/SSAO/GLSLAmbientOcclusion.vert", GLShaderType::GLST_VERTEX);
        mSSAO.compileShader("../GLRenderAPI/Shaders/PostProcess/SSAO/GLSLAmbientOcclusion.frag", GLShaderType::GLST_FRAGMENT);
        mSSAO.link();
        mSSAO.validate();

        mUniform.gViewPosition = mSSAO.getUniformLocation("gViewPosition");
        mUniform.gNormal = mSSAO.getUniformLocation("gNormal");
        mUniform.Noise = mSSAO.getUniformLocation("Noise");

        mUniform.samplesCount = mSSAO.getUniformLocation("samplesCount");
        mUniform.noiseScale = mSSAO.getUniformLocation("noiseScale");
        mUniform.radius = mSSAO.getUniformLocation("radius");

        mUniform.View = mSSAO.getUniformLocation("View");
        mUniform.Proj = mSSAO.getUniformLocation("Proj");

        for (UINT32 i = 0; i < SSAOInfo::SSAO_KERNEL_SIZE; ++i)
        {
            CHAR buffer[20];
            sprintf(buffer, "samples[%u]", i);
            mUniform.samples[i] = mSSAO.getUniformLocation(buffer);
        }

        mBlur.init();
        mBlur.compileShader("../GLRenderAPI/Shaders/PostProcess/SSAO/GLSLBlur.vert", GLShaderType::GLST_VERTEX);
        mBlur.compileShader("../GLRenderAPI/Shaders/PostProcess/SSAO/GLSLBlur.frag", GLShaderType::GLST_FRAGMENT);
        mBlur.link();
        mBlur.validate();

        mUniform.Input = mBlur.getUniformLocation("Input");

        /// SSAO Noise

        const UINT32 NOISE_SIZE = SSAOInfo::SSAO_NOISE_SIZE;
        mNoise.create(NOISE_SIZE);
        mNoiseFactor = (FLOAT32)1.0 / (FLOAT32)NOISE_SIZE;
        mNoiseScale = Vector2f(mNoiseFactor * (FLOAT32)gRenderSystem->getPixelWindowWidth(),
                               mNoiseFactor * (FLOAT32)gRenderSystem->getPixelWindowHeight());

        /// SSAO Buffer size

        auto PartOfScreenSize = gRenderSystem->getSSAOBufferSize();
        mWidth = (UINT32)(PartOfScreenSize * gRenderSystem->getPixelWindowWidth());
        mHeight = (UINT32)(PartOfScreenSize * gRenderSystem->getPixelWindowHeight());

        mBuffer.create(mWidth, mHeight);

        /// SSAO Kernel space

        for (UINT32 i = 0; i < SSAOInfo::SSAO_KERNEL_SIZE; ++i)
        {
            mKernel[i] = Vector3f(random(-1.0f, 1.0f),
                                  random(-1.0f, 1.0f),
                                  random( 0.0f, 1.0f));

            mKernel[i].normalize();

            FLOAT32 part = (FLOAT32)i / (FLOAT32)SSAOInfo::SSAO_KERNEL_SIZE;
            FLOAT32 scale = 0.1f + part * part * (1.0f - 0.1f);

            mKernel[i] = mKernel[i] * scale;
        }

        /*

        debug.init();
        debug.compileShader("../GLRenderAPI/Shaders/PreProcess/Deferred/Debug.vert", GLShaderType::GLST_VERTEX);
        debug.compileShader("../GLRenderAPI/Shaders/PreProcess/Deferred/Debug.frag", GLShaderType::GLST_FRAGMENT);
        debug.link();
        debug.validate();

         */
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

            auto PartOfScreenSize = render->getSSAOBufferSize();
            mWidth  = (UINT32)(PartOfScreenSize * gRenderSystem->getPixelWindowWidth());
            mHeight = (UINT32)(PartOfScreenSize * gRenderSystem->getPixelWindowHeight());

            mBuffer.destroy();
            mBuffer.create(mWidth, mHeight);
        }

        Matrix4x4f& View = render->getRenderCamera()->getComponent()->mView;
        Matrix4x4f& Proj = render->getRenderCamera()->getComponent()->mProjection;

        GBuffer* gBuffer = render->getGBuffer();

        // Gen SSAO buffer

        mBuffer.useAsFBO();

        driver->setClearColor(Vector4f(0.0));
        driver->clearColorBuffer();
        driver->enableDepthTest(false);
        driver->setViewPort(0, 0, mWidth, mHeight);
        driver->enableFaceCulling(true);
        driver->setBackCulling();
        driver->setWindingOrderCCW();

        mSSAO.use();

        mSSAO.setUniform(mUniform.gViewPosition, 0);
        mSSAO.setUniform(mUniform.gNormal, 1);
        mSSAO.setUniform(mUniform.Noise, 2);

        mSSAO.setUniform(mUniform.samplesCount, (UINT32)SSAOInfo::SSAO_KERNEL_SIZE);
        mSSAO.setUniform(mUniform.noiseScale, mNoiseScale);
        mSSAO.setUniform(mUniform.radius, render->getSSAORadius());

        for (UINT32 i = 0; i < SSAOInfo::SSAO_KERNEL_SIZE; i++)
        {
            mSSAO.setUniform(mUniform.samples[i], mKernel[i]);
        }

        mSSAO.setUniform(mUniform.View, View);
        mSSAO.setUniform(mUniform.Proj, Proj);

        gBuffer->useAsUniformLayout(GBufferInfo::GBI_VIEW_POSITION_SLOT, 0);
        gBuffer->useAsUniformLayout(GBufferInfo::GBI_NORMAL_SLOT, 1);
        mNoise.useAsUniform(2);

        render->getScreenPlane()->use();

        // Blur SSAO buffer

        render->getSSAOBuffer()->useAsFBO();

        mBlur.use();
        mBlur.setUniform(mUniform.Input, 0);

        mBuffer.useAsUniform(0);

        render->getScreenPlane()->use();

        /*
        return;

        const CameraComponent::Viewport& Port = render->getRenderCamera()->getComponent()->mViewport;
        driver->setDefaultBuffer();
        driver->clearBuffer();
        driver->setViewPort(Port.posX, Port.posY, Port.width, Port.height);

        debug.use();
        debug.setUniform("Screen", 0);
        render->getSSAOBuffer()->useAsUniform(0);

        render->getScreenPlane()->use();

         */
    }

} //namespace Berserk