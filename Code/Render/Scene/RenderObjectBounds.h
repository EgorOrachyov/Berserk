/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RENDEROBJECTBOUNDS_H
#define BERSERK_RENDEROBJECTBOUNDS_H

#include <Math/Aabbf.h>

namespace Berserk {
    namespace Render {

        /** Represents object bounds for occlusion culling */
        struct RenderObjectBounds {
            /** World-space aabb */
            Aabbf aabb;
            /** Min draw distance from camera view point */
            float minDrawDistance;
            /** Max draw distance from camera view point */
            float maxDrawDistance;
        };

    }
}

#endif //BERSERK_RENDEROBJECTBOUNDS_H
