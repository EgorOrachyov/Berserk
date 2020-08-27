/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_STATICMESHLOD_H
#define BERSERK_STATICMESHLOD_H

#include <Scene/StaticMeshNode.h>
#include <RHI/RHIDefinitions.h>
#include <GpuMeshAttribute.h>

namespace Berserk {
    namespace Render {

        /** Represent mesh lod - data and nodes for single lod levels */
        struct StaticMeshLod {
            MeshFormat format;
            EPrimitivesType primitivesType;
            uint32 verticesCount;
            uint32 nodesOffset;
            uint32 nodesCount;
            TPtrShared<RHIVertexBuffer> vertexData;
        };

    }
}

#endif //BERSERK_STATICMESHLOD_H
