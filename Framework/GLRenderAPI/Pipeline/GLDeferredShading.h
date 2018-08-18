//
// Created by Egor Orachyov on 10.08.2018.
//

#ifndef BERSERKENGINE_GLDEFERREDSHADING_H
#define BERSERKENGINE_GLDEFERREDSHADING_H

#include "Render/PipelineStage.h"
#include "GPUProgram/GLGPUProgram.h"
#include "Uniform/UniformInfo.h"

namespace Berserk
{

    class GLDeferredShading : public PipelineStage
    {
    private:

        class UniformData : public UDeferred
        {
        public:

            UINT32 VertPass_PN;
            UINT32 VertPass_PNT;
            UINT32 VertPass_PNBTT;

            UINT32 FragPass_PN;
            UINT32 FragPass_PNT;
            UINT32 FragPass_PNBTT;

            UINT32 MatPass_Default;
            UINT32 MatPass_Basic;
            UINT32 MatPass_DS_map;
        };

    public:

        virtual ~GLDeferredShading() = default;

        void init() override;
        void destroy() override;
        void execute() override;

    private:

        UniformData mUniform;
        GLGPUProgram mProgram;

        GLGPUProgram debug;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLDEFERREDSHADING_H