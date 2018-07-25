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

        void preProcess(CameraInfo* camera,
                        SpotLightInfo* spot, UINT32 spotCount,
                        DirectionalLightInfo* direct, UINT32 directCount,
                        PointLightInfo* point, UINT32 pointCount,
                        GLGPUBuffer* buffers, MaterialInfo* materials, UINT32 count) override;

        void mainProcess(CameraInfo* camera,
                         SpotLightInfo* spot, UINT32 spotCount,
                         DirectionalLightInfo* direct, UINT32 directCount,
                         PointLightInfo* point, UINT32 pointCount,
                         GLGPUBuffer* buffers, MaterialInfo* materials, UINT32 count) override;

        void postProcess(CameraInfo* camera,
                         SpotLightInfo* spot, UINT32 spotCount,
                         DirectionalLightInfo* direct, UINT32 directCount,
                         PointLightInfo* point, UINT32 pointCount,
                         GLGPUBuffer* buffers, MaterialInfo* materials, UINT32 count) override;

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