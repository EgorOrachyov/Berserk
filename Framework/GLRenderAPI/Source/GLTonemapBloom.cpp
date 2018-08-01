//
// Created by Egor Orachyov on 01.08.2018.
//

#include "Pipeline/GLTonemapBloom.h"
#include <cmath>

namespace Berserk
{
    FLOAT32 gauss(FLOAT32 x, FLOAT32 sigma2 )
    {
        FLOAT64 coeff = 1.0 / (2 * M_PI * sigma2);
        FLOAT64 expon = -(x*x) / (2.0 * sigma2);
        return (FLOAT32) (coeff * exp(expon));
    }

    GLTonemapBloom::GLTonemapBloom()
    {

    }

    GLTonemapBloom::~GLTonemapBloom()
    {
        destroy();
    }

    void GLTonemapBloom::init()
    {
        mProgram.init();
        mProgram.init();
        mProgram.compileShader("../GLRenderAPI/Shaders/PostProcess/GLSLTonemapBloom.vert", GLShaderType::GLST_VERTEX);
        mProgram.compileShader("../GLRenderAPI/Shaders/PostProcess/GLSLTonemapBloom.frag", GLShaderType::GLST_FRAGMENT);
        mProgram.link();
        mProgram.validate();

        mUniform.HDRTex = mProgram.getUniformLocation("HDRTex");
        mUniform.BlurTex1 = mProgram.getUniformLocation("BlurTex1");
        mUniform.BlurTex2 = mProgram.getUniformLocation("BlurTex2");

        mUniform.LuminanceThresh = mProgram.getUniformLocation("LuminanceThresh");
        mUniform.Exposure = mProgram.getUniformLocation("Exposure");
        mUniform.White = mProgram.getUniformLocation("White");
        mUniform.AverageLuminance = mProgram.getUniformLocation("AverageLuminance");

        mUniform.pass1 = mProgram.getSubroutineIndex("pass1", GLShaderType::GLST_FRAGMENT);
        mUniform.pass2 = mProgram.getSubroutineIndex("pass2", GLShaderType::GLST_FRAGMENT);
        mUniform.pass3 = mProgram.getSubroutineIndex("pass3", GLShaderType::GLST_FRAGMENT);
        mUniform.pass4 = mProgram.getSubroutineIndex("pass4", GLShaderType::GLST_FRAGMENT);

        for(UINT32 i = 0; i < 10; i++)
        {
            CHAR buffer[BUFFER_SIZE_16] = {'\0'};
            sprintf(buffer, "Weight[%u]", i);
            mUniform.Weight[i] = mProgram.getUniformLocation(buffer);
        }

        // todo: init via cfg table
        mWhite = 0.928;
        mExposure = 0.35;
        mLuminanceThresh = 0.75;
        mSigma2 = 25.0;

        FLOAT32 sum;
        mWeight[0] = gauss(0,mSigma2);
        sum = mWeight[0];
        for (UINT32 i = 1; i < 10; i++ )
        {
            mWeight[i] = gauss(FLOAT32(i), mSigma2);
            sum += 2 * mWeight[i];
        }

        for (UINT32 i = 1; i < 10; i++ )
        {
            mWeight[i] /= sum;
        }

        // todo: get real width and height of the window and init fbo objects
    }

    void GLTonemapBloom::destroy()
    {
        mProgram.destroy();
    }

    GLFrameBufferObject* GLTonemapBloom::process(RenderManager *manager, GLFrameBufferObject *object)
    {
        mProgram.use();
        glDisable(GL_DEPTH_TEST);

        object->useAsUniformData();
        mBlurTex1.useAsFBO();
        pass1();

        mBlurTex1.useAsUniformData();
        mBlurTex2.useAsFBO();
        pass2();

        mBlurTex1.useAsFBO();
        mBlurTex2.useAsUniformData();
        pass3();

        mBlurTex1.disable();
        mBlurTex1.useAsUniformData();
        object->useAsUniformData();
        pass4();
    }

    void GLTonemapBloom::pass1()
    {
        mProgram.setSubroutines(GLShaderType::GLST_FRAGMENT, 1, &mUniform.pass1);

        mProgram.setUniform(mUniform.HDRTex, 0);
        mProgram.setUniform(mUniform.BlurTex1, 1);
        mProgram.setUniform(mUniform.LuminanceThresh, mLuminanceThresh);

        // draw rect
    }

    void GLTonemapBloom::pass2()
    {
        mProgram.setSubroutines(GLShaderType::GLST_FRAGMENT, 1, &mUniform.pass2);

        mProgram.setUniform(mUniform.BlurTex1, 1);
        mProgram.setUniform(mUniform.BlurTex2, 2);

        for (UINT32 i = 0; i < 10; i++)
        {
            mProgram.setUniform(mUniform.Weight[i], mWeight[i]);
        }

        // draw rect
    }

    void GLTonemapBloom::pass3()
    {
        mProgram.setSubroutines(GLShaderType::GLST_FRAGMENT, 1, &mUniform.pass3);

        mProgram.setUniform(mUniform.BlurTex1, 1);
        mProgram.setUniform(mUniform.BlurTex2, 2);

        for (UINT32 i = 0; i < 10; i++)
        {
            mProgram.setUniform(mUniform.Weight[i], mWeight[i]);
        }

        // draw rect
    }

    void GLTonemapBloom::pass4()
    {
        mProgram.setSubroutines(GLShaderType::GLST_FRAGMENT, 1, &mUniform.pass4);

        mProgram.setUniform(mUniform.HDRTex, 0);
        mProgram.setUniform(mUniform.BlurTex1, 1);

        mProgram.setUniform(mUniform.Exposure, mExposure);
        mProgram.setUniform(mUniform.White, mWhite);

        // Compute log average luminance

        // draw rect
    }

} // namespace Berserk