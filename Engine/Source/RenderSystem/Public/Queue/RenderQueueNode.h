//
// Created by Egor Orachyov on 15.04.2019.
//

#ifndef BERSERK_RENDERQUEUENODE_H
#define BERSERK_RENDERQUEUENODE_H

#include <Components/MeshComponent.h>
#include <Components/IPrimitiveComponent.h>

namespace Berserk::Render
{

    /** Render Queue data */
    class ENGINE_API RenderQueueNode
    {
    public:

        /** The primitive which is submitted [Must be valid for current frame] */
        class Engine::IPrimitiveComponent* mRootComponent = nullptr;

        /** One Render engine unit to drawn [Must be valid for current frame] */
        Engine::MeshComponent mMeshComponent;

        /** Distance from the camera (for depth sort) */
        float32 mViewDepth = 0.0f;

        /** World transformation */
        Mat4x4f mTransformation;

        /**
         * Compare predicate to define sorting order in render queue.
         * If materials are not equal, order via materials.
         * If materials are the same, sort in via depth (if depth less, this
         * one will be earlier in buffer than other)
         *
         * @param other Right operand
         * @return True, if this < other
         */
        const bool operator < (const RenderQueueNode& other) const
        {
            if (mMeshComponent.mRenderMaterial != other.mMeshComponent.mRenderMaterial)
            {
                return (mMeshComponent.mRenderMaterial < other.mMeshComponent.mRenderMaterial);
            }
            else
            {
                return (mViewDepth < other.mViewDepth);
            }
        }

    };

} // namespace Berserk::Render

#endif //BERSERK_RENDERQUEUENODE_H