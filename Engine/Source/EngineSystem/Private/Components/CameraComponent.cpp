//
// Created by Egor Orachyov on 15.04.2019.
//

#include "Components/CameraComponent.h"

namespace Berserk::Engine
{

    CameraComponent::CameraComponent(const IObjectInitializer &objectInitializer)
            : SceneComponent(objectInitializer),
              mIsPerspective(FIELD_OFF),
              mIsOrthographic(FIELD_OFF)
    {

    }

    void CameraComponent::setProjection(float32 angle, float32 aspect, float32 near, float32 far)
    {
        mIsPerspective = FIELD_ON;

        mViewAngle = angle;
        mViewAspect = aspect;

        mNearViewDistance = near;
        mFarViewDistance = far;
        mFarViewDistanceSq = mFarViewDistance * mFarViewDistance;

        mProjection = Mat4x4f::perspective(angle, aspect, near, far);
    }

    void CameraComponent::setProjection(float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far)
    {
        mIsOrthographic = FIELD_ON;

        mOrthoViewLeft = left;
        mOrthoViewRight = right;
        mOrthoViewTop = top;
        mOrthoViewBottom = bottom;

        mNearViewDistance = near;
        mFarViewDistance = far;
        mFarViewDistanceSq = mFarViewDistance * mFarViewDistance;

        mProjection = Mat4x4f::orthographic(left, right, bottom, top, near, far);
    }

    void CameraComponent::onTransformUpdate()
    {
        PUSH("It is camera component '%s'", mObjectName.get());

        mLocalPosition = Vec3f(toLocalSpace() * Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
        mLocalDirection = Vec3f(toLocalSpace() * Vec4f(Vec3f::axisZ, 0.0f));
        mLocalUp = Vec3f(toLocalSpace() * Vec4f(Vec3f::axisY, 0.0f));

        mWorldPosition = Vec3f(toGlobalSpace() * Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
        mWorldDirection = Vec3f(toGlobalSpace() * Vec4f(Vec3f::axisZ, 0.0f));
        mWorldUp = Vec3f(toGlobalSpace() * Vec4f(Vec3f::axisY, 0.0f));

        mView = Mat4x4f::lookAt(mWorldPosition, mWorldDirection, mWorldUp);

        updatePlanes();
    }

    void CameraComponent::updatePlanes()
    {
        if (mIsPerspective)
        {
            // Recalculate perspective frustum
            // from default properties

            mFrustum = Frustum(mViewAngle, mViewAspect,
                               mNearViewDistance, mFarViewDistance,
                               mWorldPosition, mWorldDirection, mWorldUp);
        }
        if (mIsOrthographic)
        {
            // Recalculate orthographic view
            // volume from default properties

            mFrustum = Frustum(mNearViewDistance, mFarViewDistance,
                               mOrthoViewBottom, mOrthoViewTop,
                               mOrthoViewLeft, mOrthoViewRight,
                               mWorldPosition, mWorldDirection, mWorldUp);
        }
    }

} // namespace Berserk::Engine