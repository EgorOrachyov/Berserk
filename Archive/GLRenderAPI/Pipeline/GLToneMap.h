//
// Created by Egor Orachyov on 10.08.2018.
//

#ifndef BERSERKENGINE_GLTONEMAP_H
#define BERSERKENGINE_GLTONEMAP_H

#include "Uniform/UniformInfo.h"
#include "Render/PipelineStage.h"
#include "GPUProgram/GLGPUProgram.h"

namespace Berserk
{

    class GLToneMap : public PipelineStage
    {
    private:

        class UniformData : public ToneMapExp
        {

        };

    public:

        virtual ~GLToneMap() = default;

        void init() override;
        void destroy() override;
        void execute() override;

    private:

        GLGPUProgram mProgram;
        UniformData  mUniform;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLTONEMAP_H