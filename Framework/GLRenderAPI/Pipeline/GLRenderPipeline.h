//
// Created by Egor Orachyov on 17.07.2018.
//

#ifndef BERSERKENGINE_GLRENDERPIPELINE_H
#define BERSERKENGINE_GLRENDERPIPELINE_H

#include "Containers/LinkedList.h"

#include "Debug/SpotLightInfo.h"
#include "Debug/DirectionalLightInfo.h"
#include "Debug/PointLightInfo.h"
#include "Debug/CameraInfo.h"
#include "Debug/MaterialInfo.h"

#include "Buffers/GLGPUBuffer.h"

// todo: add fully featured render queue to handle draw requests, buffers and different materials' types
#define MAX_SHADOW_MAPS 10

namespace Berserk
{

    class GLRenderPipeline
    {
    public:

        virtual void init() = 0;
        virtual void destroy() = 0;

        virtual void preProcess(CameraInfo* camera,
                                SpotLightInfo* spot, UINT32 spotCount,
                                DirectionalLightInfo* direct, UINT32 directCount,
                                PointLightInfo* point, UINT32 pointCount,
                                GLGPUBuffer* buffers, MaterialInfo* materials, UINT32 count) = 0;

        virtual void mainProcess(CameraInfo* camera,
                                 SpotLightInfo* spot, UINT32 spotCount,
                                 DirectionalLightInfo* direct, UINT32 directCount,
                                 PointLightInfo* point, UINT32 pointCount,
                                 GLGPUBuffer* buffers, MaterialInfo* materials, UINT32 count) = 0;

        virtual void postProcess(CameraInfo* camera,
                                 SpotLightInfo* spot, UINT32 spotCount,
                                 DirectionalLightInfo* direct, UINT32 directCount,
                                 PointLightInfo* point, UINT32 pointCount,
                                 GLGPUBuffer* buffers, MaterialInfo* materials, UINT32 count) = 0;

    };

}

#endif //BERSERKENGINE_GLRENDERPIPELINE_H