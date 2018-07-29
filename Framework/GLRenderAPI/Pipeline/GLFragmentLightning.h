//
// Created by Egor Orachyov on 29.07.2018.
//

#ifndef BERSERKENGINE_GLFRAGMENTLIGHTNING_H
#define BERSERKENGINE_GLFRAGMENTLIGHTNING_H

#include "GLRenderPipeline.h"

namespace Berserk
{

    class GLFragmentLightning : public GLRenderPipeline
    {
    public:

        GLFragmentLightning();
        virtual ~GLFragmentLightning();

        void init() override;
        void destroy() override;
        GLFrameBufferObject* process(RenderManager *manager, GLFrameBufferObject *object) override;

    private:

        GLGPUProgram mProgram;

    };

}

#endif //BERSERKENGINE_GLFRAGMENTLIGHTNING_H