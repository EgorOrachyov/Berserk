//
// Created by Egor Orachyov on 16.04.2019.
//

#ifndef BERSERK_SUBMITQUEUENODE_H
#define BERSERK_SUBMITQUEUENODE_H

#include <Components/MeshComponent.h>
#include <Components/IPrimitiveComponent.h>

namespace Berserk::Render
{

    /** Submit queue data */
    class ENGINE_API SubmitQueueNode
    {
    public:

        /** World transformation */
        Mat4x4f mTransformation;

        /** Used for frustum culling */
        AABB mBoundingBox;

        /** The primitive which is submitted [Must be valid for current frame] */
        class Engine::IPrimitiveComponent* mRootComponent = nullptr;

        /** Render data buffer [Must be valid for current frame] */
        class Engine::MeshComponent* mBuffer = nullptr;

        /** Count of the meshes in the buffer */
        uint32 mBufferSize = 0;

        /** Distance from the camera (for depth sort) */
        float32 mViewDepth = 0.0f;

        /** Stores non-zero value if the node in the frustum */
        mutable float32 mIsInFrustum = 0.0f;

    };

} // namespace Berserk::Render

#endif //BERSERK_SUBMITQUEUENODE_H