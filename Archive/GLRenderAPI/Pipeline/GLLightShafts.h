//
// Created by Egor Orachyov on 26.08.2018.
//

#ifndef BERSERKENGINE_GLLIGHTSHAFTS_H
#define BERSERKENGINE_GLLIGHTSHAFTS_H

#include "Render/PipelineStage.h"
#include "GPUProgram/GLGPUProgram.h"
#include "Uniform/UniformInfo.h"
#include "Buffers/GLLightMap.h"

namespace Berserk
{

    class GLLightShafts : public PipelineStage
    {
    private:

        class UniformData
        {
        public:

            INT32 Depth;
            INT32 LightColor;
            INT32 LightBackColor;
            INT32 LightPosition;
            INT32 LightRadius;

            INT32 Exposure;
            INT32 Decay;
            INT32 Density;
            INT32 Weight;
            INT32 Position;
            INT32 LightMap;

            INT32 Input;
            INT32 Blend;
        };


    public:

        virtual ~GLLightShafts() = default;

        void init() override;
        void destroy() override;
        void execute() override;

    private:

        UINT32  mWidth;
        UINT32  mHeight;

        UniformData mUniform;

        GLGPUProgram mScreenMap;
        GLGPUProgram mRadialBlur;
        GLGPUProgram mBlurBlend;

        GLLightMap mBuffer1;
        GLLightMap mBuffer2;
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLLIGHTSHAFTS_H