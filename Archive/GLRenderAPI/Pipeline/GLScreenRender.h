//
// Created by Egor Orachyov on 10.08.2018.
//

#ifndef BERSERKENGINE_GLSCREENRENDER_H
#define BERSERKENGINE_GLSCREENRENDER_H

#include "Uniform/UniformInfo.h"
#include "Render/PipelineStage.h"
#include "GPUProgram/GLGPUProgram.h"

namespace Berserk
{

    class GLScreenRender : public PipelineStage
    {
    public:

        virtual ~GLScreenRender() = default;

        void init() override;
        void destroy() override;
        void execute() override;

    private:

        GLGPUProgram mProgram;
        INT32 mUniform;
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLSCREENRENDER_H