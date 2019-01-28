//
// Created by Egor Orachyov on 21.08.2018.
//

#ifndef BERSERKENGINE_GLAMBIENTOCCLUSION_H
#define BERSERKENGINE_GLAMBIENTOCCLUSION_H

#include "Render/PipelineStage.h"
#include "GPUProgram/GLGPUProgram.h"
#include "Uniform/UniformInfo.h"
#include "Buffers/GLSSAONoise.h"
#include "Buffers/GLSSAOBuffer.h"

namespace Berserk
{

    class GLAmbientOcclusion : public PipelineStage
    {
    private:

        class UniformData
        {
        public:

            INT32 gViewPosition;
            INT32 gNormal;
            INT32 Noise;

            INT32 samplesCount;
            INT32 samples[SSAOInfo::SSAO_KERNEL_SIZE];
            INT32 noiseScale;
            INT32 radius;

            INT32 View;
            INT32 Proj;

            INT32 Input;
        };

    public:

        virtual ~GLAmbientOcclusion() = default;

        void init() override;
        void destroy() override;
        void execute() override;

    private:

        UINT32 mWidth;
        UINT32 mHeight;

        GLSSAONoise  mNoise;
        FLOAT32      mNoiseFactor;
        Vector2f     mNoiseScale;
        Vector3f     mKernel[SSAOInfo::SSAO_KERNEL_SIZE];

        UniformData  mUniform;
        GLGPUProgram mSSAO;
        GLGPUProgram mBlur;
        //GLGPUProgram debug;

        GLSSAOBuffer mBuffer;
    };

} //namespace Berserk

#endif //BERSERKENGINE_GLAMBIENTOCCLUSION_H