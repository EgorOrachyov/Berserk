//
// Created by Egor Orachyov on 17.07.2018.
//

#ifndef BERSERKENGINE_GLRENDERPIPELINE_H
#define BERSERKENGINE_GLRENDERPIPELINE_H

#include "Containers/LinkedList.h"

namespace Berserk
{

    class GLRenderPipeline
    {
    public:

        virtual void init() = 0;
        virtual void destroy() = 0;

        virtual void preProcess() = 0;
        virtual void mainProcess() = 0;
        virtual void postProcess() = 0;

    };

}

#endif //BERSERKENGINE_GLRENDERPIPELINE_H