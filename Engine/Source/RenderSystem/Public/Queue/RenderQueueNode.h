//
// Created by Egor Orachyov on 15.04.2019.
//

#ifndef BERSERK_RENDERQUEUENODE_H
#define BERSERK_RENDERQUEUENODE_H

#include <Platform/IGPUBuffer.h>
#include <Foundation/IMaterial.h>

namespace Berserk::Render
{

    /** Render Queue data */
    class ENGINE_API RenderQueueNode
    {
    public:

        /** Render data buffer [should be valid for current frame] */
        Resources::IGPUBuffer* mBuffer = nullptr;

        /** Applied material [should be valid for current frame] */
        Resources::IMaterial* mMaterial = nullptr;

        /** World transformation */
        Mat4x4f mTransformation;

        /** Used for frustum culling */
        AABB mBoundingBox;

        /** Stores non-zero value if the node in the frustum */
        float32 mIsInFrustum = 0.0f;

    };

} // namespace Berserk::Render

#endif //BERSERK_RENDERQUEUENODE_H