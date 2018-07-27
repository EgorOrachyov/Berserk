//
// Created by Egor Orachyov on 18.07.2018.
//

#ifndef BERSERKENGINE_GLSHADOWMAPHDRBLOOMPIPELINE_H
#define BERSERKENGINE_GLSHADOWMAPHDRBLOOMPIPELINE_H

#include "GLRenderPipeline.h"
#include "GPUProgram/GLGPUProgram.h"
#include "Buffers/GLFrameBufferObject.h"

namespace Berserk
{

    class GLShadowMapHDRBloomPipeline : public GLRenderPipeline
    {
    public:

        void init() override;
        void destroy() override;

    private:

        GLFrameBufferObject mHDRbuffer;
        GLFrameBufferObject mBloomPass1;
        GLFrameBufferObject mBloomPass2;
        //GLFrameBufferObject mShadowMaps[MAX_SHADOW_MAPS];

        //GLGPUProgram mShadowMapCreator;
        GLGPUProgram mHDRBufferCreator;
        GLGPUProgram mBloomMapCreator;
        GLGPUProgram mBloomPass1Creator;
        GLGPUProgram mBloomPass2Creator;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLSHADOWMAPHDRBLOOMPIPELINE_H