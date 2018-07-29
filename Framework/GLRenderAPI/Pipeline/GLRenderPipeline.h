//
// Created by Egor Orachyov on 17.07.2018.
//

#ifndef BERSERKENGINE_GLRENDERPIPELINE_H
#define BERSERKENGINE_GLRENDERPIPELINE_H

#include "Managers/RenderManager.h"
#include "GPUProgram/GLGPUProgram.h"
#include "Buffers/GLFrameBufferObject.h"

namespace Berserk
{

    class GLRenderPipeline
    {
    public:

        virtual ~GLRenderPipeline() = default;
        virtual void init() = 0;
        virtual void destroy() = 0;
        virtual GLFrameBufferObject* process(RenderManager *manager, GLFrameBufferObject *object) = 0;

    };

}

#endif //BERSERKENGINE_GLRENDERPIPELINE_H