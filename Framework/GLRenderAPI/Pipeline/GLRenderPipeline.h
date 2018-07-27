//
// Created by Egor Orachyov on 17.07.2018.
//

#ifndef BERSERKENGINE_GLRENDERPIPELINE_H
#define BERSERKENGINE_GLRENDERPIPELINE_H

#include "Containers/LinkedList.h"

#include "Buffers/GLGPUBuffer.h"

namespace Berserk
{

    class GLRenderPipeline
    {
    public:

        virtual void init() = 0;
        virtual void destroy() = 0;

    };

}

#endif //BERSERKENGINE_GLRENDERPIPELINE_H