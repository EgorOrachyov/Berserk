//
// Created by Egor Orachyov on 10.08.2018.
//

#ifndef BERSERKENGINE_GLGAUSSIANBLOOM_H
#define BERSERKENGINE_GLGAUSSIANBLOOM_H

#include "Uniform/UniformInfo.h"
#include "Render/PipelineStage.h"
#include "GPUProgram/GLGPUProgram.h"
#include "Buffers/GLFrameBufferObject.h"

namespace Berserk
{

    class GLGaussianBloom : public PipelineStage
    {
    private:

        class UniformData : public GaussianBloom5
        {
        public:
            UINT32 pass1;
            UINT32 pass2;
            UINT32 pass3;
            UINT32 pass4;
        };

    public:

        virtual ~GLGaussianBloom() = default;

        void init() override;
        void destroy() override;
        void execute() override;

    private:

        FLOAT32 mPartOfScreenSize;
        UINT32  mWidth;
        UINT32  mHeight;

        UniformData mUniform;
        GLGPUProgram mProgram;

        GLFrameBufferObject mBlurTex1;
        GLFrameBufferObject mBlurTex2;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLGAUSSIANBLOOM_H