/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Scene/RenderObject.h>

namespace Berserk {
    namespace Render {

        RenderObject::RenderObject() {
            mCastShadows = false;
            mVisible = false;
            mHasVelocity = false;
            mCanBeCulled = true;
        }

        void RenderObject::setTransform(const Transformf &t) {
            mTransform = t;
            mCachedLocalToWorld = mTransform.toTransformMat();

            TArrayStatic<TVecN<float,3>,8> points;
            mBounds.getPoints(points);

            for (auto& point: points) {
                point = mTransform.transform(point);
            }

            mCachedWorldBounds = Aabbf(points);
        }

        void RenderObject::getBounds(RenderObjectBounds &bounds) {
            bounds.maxDrawDistance = mMaxViewDistance;
            bounds.minDrawDistance = mMinViewDistance;
            bounds.aabb = mCachedWorldBounds;
        }

    }
}