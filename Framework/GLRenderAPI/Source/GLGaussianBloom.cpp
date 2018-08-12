//
// Created by Egor Orachyov on 10.08.2018.
//

#include "Pipeline/GLGaussianBloom.h"
#include "Render/GLRenderDriver.h"
#include "Render/GLRenderSystem.h"

namespace Berserk
{

    void GLGaussianBloom::init()
    {
        mProgram.init();
        mProgram.compileShader("../GLRenderAPI/Shaders/PostProcess/GLSLGaussianBloom.vert", GLShaderType::GLST_VERTEX);
        mProgram.compileShader("../GLRenderAPI/Shaders/PostProcess/GLSLGaussianBloom.frag", GLShaderType::GLST_FRAGMENT);
        mProgram.link();
        mProgram.validate();

        mUniform.HDRTex = mProgram.getUniformLocation("HDRTex");
        mUniform.BlurTex1 = mProgram.getUniformLocation("BlurTex1");
        mUniform.BlurTex2 = mProgram.getUniformLocation("BlurTex2");

        mUniform.LuminanceThresh = mProgram.getUniformLocation("LuminanceThresh");

        mUniform.pass1 = mProgram.getSubroutineIndex("pass1", GLShaderType::GLST_FRAGMENT);
        mUniform.pass2 = mProgram.getSubroutineIndex("pass2", GLShaderType::GLST_FRAGMENT);
        mUniform.pass3 = mProgram.getSubroutineIndex("pass3", GLShaderType::GLST_FRAGMENT);
        mUniform.pass4 = mProgram.getSubroutineIndex("pass4", GLShaderType::GLST_FRAGMENT);

        mPartOfScreenSize = (FLOAT32)1 / 10;
        mWidth = (UINT32)(mPartOfScreenSize * gRenderSystem->getPixelWindowWidth());
        mHeight = (UINT32)(mPartOfScreenSize * gRenderSystem->getPixelWindowHeight());

        mBlurTex1.init(mWidth, mHeight);
        mBlurTex1.addTexture(GLInternalTextureFormat::GLTF_RGB32F, GLWrapping::GLW_CLAMP_TO_EDGE, GLFiltering::GLF_LINEAR, 0, 1);
        mBlurTex1.setShaderAttachments();

        mBlurTex2.init(mWidth, mHeight);
        mBlurTex2.addTexture(GLInternalTextureFormat::GLTF_RGB32F, GLWrapping::GLW_CLAMP_TO_EDGE, GLFiltering::GLF_LINEAR, 0, 2);
        mBlurTex2.setShaderAttachments();
    }

    void GLGaussianBloom::destroy()
    {
        mProgram.destroy();
        mBlurTex1.destroy();
        mBlurTex2.destroy();
    }

    void GLGaussianBloom::execute()
    {
        auto render = dynamic_cast<GLRenderSystem*>(gRenderSystem);
        auto driver = dynamic_cast<GLRenderDriver*>(gRenderDriver);

        if (render->wasReSized())
        {
            mWidth = (UINT32)(mPartOfScreenSize * gRenderSystem->getPixelWindowWidth());
            mHeight = (UINT32)(mPartOfScreenSize * gRenderSystem->getPixelWindowHeight());

            mBlurTex1.destroy();
            mBlurTex1.init(mWidth, mHeight);
            mBlurTex1.addTexture(GLInternalTextureFormat::GLTF_RGB32F, GLWrapping::GLW_CLAMP_TO_EDGE, GLFiltering::GLF_LINEAR, 0, 1);
            mBlurTex1.setShaderAttachments();

            mBlurTex2.destroy();
            mBlurTex2.init(mWidth, mHeight);
            mBlurTex2.addTexture(GLInternalTextureFormat::GLTF_RGB32F, GLWrapping::GLW_CLAMP_TO_EDGE, GLFiltering::GLF_LINEAR, 0, 2);
            mBlurTex2.setShaderAttachments();
        }

        mProgram.use();
        mProgram.setUniform(mUniform.HDRTex, 0);
        mProgram.setUniform(mUniform.BlurTex1, 1);
        mProgram.setUniform(mUniform.BlurTex2, 2);
        mProgram.setUniform(mUniform.LuminanceThresh, render->getLuminanceThresh());

        render->getStageInBuffer()->useAsUniformData();
        mBlurTex1.useAsFBO();
        driver->enableDepthTest(false);
        driver->setViewPort(0, 0, mWidth, mHeight);
        mProgram.setSubroutines(GLShaderType::GLST_FRAGMENT, 1, &mUniform.pass1);
        render->getScreenPlane()->use();

        for(UINT32 i = 0; i < 1; i++)
        {
            mBlurTex1.useAsUniformData();
            mBlurTex2.useAsFBO();
            mProgram.setSubroutines(GLShaderType::GLST_FRAGMENT, 1, &mUniform.pass2);
            render->getScreenPlane()->use();

            mBlurTex1.useAsFBO();
            mBlurTex2.useAsUniformData();
            mProgram.setSubroutines(GLShaderType::GLST_FRAGMENT, 1, &mUniform.pass3);
            render->getScreenPlane()->use();
        }

        render->getStageInBuffer()->useAsUniformData();
        render->getStageOutBuffer()->useAsFBO();
        driver->enableDepthTest(false);
        driver->setViewPort(0, 0, gRenderSystem->getPixelWindowWidth(), gRenderSystem->getPixelWindowHeight());
        mBlurTex1.useAsUniformData();
        mProgram.setSubroutines(GLShaderType::GLST_FRAGMENT, 1, &mUniform.pass4);
        render->getScreenPlane()->use();

        render->getStageOutBuffer()->disable();
    }

} // namespace Berserk