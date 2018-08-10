//
// Created by Egor Orachyov on 10.08.2018.
//

#ifndef BERSERKENGINE_GLPHONGMODEL_H
#define BERSERKENGINE_GLPHONGMODEL_H

#include "Render/PipelineStage.h"
#include "GPUProgram/GLGPUProgram.h"
#include "Uniform/UniformInfo.h"

namespace Berserk
{

    class GLPhongModel : public PipelineStage
    {
    private:

        class UniformData : public UBaseTransformation, public ULightsInfo
        {
        public:
            INT32 PassType;
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

#endif //BERSERKENGINE_GLPHONGMODEL_H