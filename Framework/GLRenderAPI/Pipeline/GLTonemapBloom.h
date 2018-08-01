//
// Created by Egor Orachyov on 01.08.2018.
//

#ifndef BERSERKENGINE_GLTONEMAPBLOOM_H
#define BERSERKENGINE_GLTONEMAPBLOOM_H

#include "GLRenderPipeline.h"
#include "Uniform/PostProcessStructures.h"

namespace Berserk
{

    class GLTonemapBloom : public GLRenderPipeline
    {
    private:

        class UniformData : public Tonemap, public GaussianBlur10
        {
        public:

            UINT32 pass1;
            UINT32 pass2;
            UINT32 pass3;
            UINT32 pass4;

            INT32 HDRTex;
            INT32 BlurTex1;
            INT32 BlurTex2;
            INT32 LuminanceThresh;
        };

    public:

        GLTonemapBloom();
        virtual ~GLTonemapBloom();

        void init() override;
        void destroy() override;
        GLFrameBufferObject* process(RenderManager *manager, GLFrameBufferObject *object) override;

    private:

        void pass1();
        void pass2();
        void pass3();
        void pass4();

    private:

        GLGPUProgram mProgram;
        UniformData  mUniform;
        GLFrameBufferObject mBlurTex1;
        GLFrameBufferObject mBlurTex2;

        FLOAT32 mLuminanceThresh;
        FLOAT32 mSigma2;
        FLOAT32 mWeight[10];

        FLOAT32 mWhite;
        FLOAT32 mExposure;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLTONEMAPBLOOM_H