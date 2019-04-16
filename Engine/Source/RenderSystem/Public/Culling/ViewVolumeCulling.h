//
// Created by Egor Orachyov on 16.04.2019.
//

#ifndef BERSERK_VIEWVOLUMECULLING_H
#define BERSERK_VIEWVOLUMECULLING_H

#include <Foundation/Forward.h>
#include <Queue/SubmitQueueNode.h>

namespace Berserk::Render
{

    /**
     * Frustum (Orthographic view)
     */
    class ENGINE_API ViewVolumeCulling
    {
        /**
         * Inside view frustum test for AABB bounded render primitives
         * @param planes Array of 6 planes
         * @param data   Buffer with data to test occlusion
         * @param size   Count of nodes to test
         */
        static void inside(const Plane *planes, const SubmitQueueNode *data, uint32 size);

        /**
         * SIMD (SSE inside view frustum test for AABB bounded render primitives
         * @param planes Array of 6 planes
         * @param data   Buffer with data to test occlusion
         * @param size   Count of nodes to test
         */
        static void inside_SIMD(const Plane *planes, const SubmitQueueNode *data, uint32 size);

    };

} // namespace Berserk::Engine

#endif //BERSERK_VIEWVOLUMECULLING_H