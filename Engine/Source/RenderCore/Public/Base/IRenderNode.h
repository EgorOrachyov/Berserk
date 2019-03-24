//
// Created by Egor Orachyov on 24.03.2019.
//

#ifndef BERSERK_IRENDERNODE_H
#define BERSERK_IRENDERNODE_H

#include "Misc/Types.h"
#include "Math/AABB.h"
#include "Base/RenderBase.h"

namespace Berserk
{

    class IRenderNode : public RenderBase
    {
    public:

        enum RenderNodeType
        {
            eRNT_NOT_RENDER_NODE = 0,

            eRNT_LIGHT_SOURCE,
            eRNT_PARTICLE_EMMITER,
            eRNT_MODEL,
            eRNT_TERRAIN,
            eRNT_WATER_VOLUME,
            eRNT_USER_INTERFACE,
            eRNT_HUD,

            eRNT_TOTAL_TYPES
        };

        enum RenderNodeFlags
        {

        };

    public:

        IRenderNode() : mType(eRNT_NOT_RENDER_NODE),
                        mMaxViewDistance(0.0f),
                        mLodStep(0.0f),
                        mRenderNodeFlags(0),
                        mBoundingBox(),
                        mTransformation()
        {

        }

        virtual void getName(char* buffer) = 0;

        virtual void getDebugInfo(char* buffer) = 0;

        uint32 getRenderNodeFlags()
        {
            return mRenderNodeFlags;
        }

        virtual Vec3f getPosition() const
        {
            return Vec3f(mTransformation.m[3],
                         mTransformation.m[7],
                         mTransformation.m[11]);
        }

        virtual AABB getAABB() const
        {
            return mBoundingBox;
        }

        virtual Mat4x4f getTransformation() const
        {
            return mTransformation;
        }

    protected:

        RenderNodeType mType;

        float32 mMaxViewDistance;
        float32 mLodStep;
        uint32  mRenderNodeFlags;

        AABB    mBoundingBox;
        Mat4x4f mTransformation;

    };

} // namespace Berserk

#endif //BERSERK_IRENDERNODE_H