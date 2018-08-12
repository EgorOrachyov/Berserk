//
// Created by Egor Orachyov on 12.08.2018.
//

#ifndef BERSERKENGINE_GLSHADOWMAP_H
#define BERSERKENGINE_GLSHADOWMAP_H

#include "Render/PipelineStage.h"
#include "GPUProgram/GLGPUProgram.h"

namespace Berserk
{

    class GLShadowMap : public PipelineStage
    {
    private:

        class UniformData
        {
        public:
            INT32 MVP;
        };

    public:

        void init() override;
        void destroy() override;
        void execute() override;

    private:

        UniformData mUniform;

        GLGPUProgram mDirectional;
        GLGPUProgram mOmnidirectional;


        GLGPUProgram debug;
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLSHADOWMAP_H