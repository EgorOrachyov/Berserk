//
// Created by Egor Orachyov on 12.08.2018.
//

#ifndef BERSERKENGINE_GLPHONGSHADOW_H
#define BERSERKENGINE_GLPHONGSHADOW_H

#include "Render/PipelineStage.h"
#include "GPUProgram/GLGPUProgram.h"
#include "Uniform/UniformInfo.h"

namespace Berserk
{

    class GLPhongShadow : public PipelineStage
    {
    private:

        class UniformData : public UBaseTransformation, public ULightsInfo, public UShadowLightsInfo
        {
        public:
            UMaterialInfo Material;
        };

    public:

        void init() override;
        void destroy() override;
        void execute() override;

    private:

        GLGPUProgram mProgram;
        UniformData  mUniform;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLPHONGSHADOW_H