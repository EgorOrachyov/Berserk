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

    void CameraComponent::updateTransformation()
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
            float32 tan = Math::tg(mViewAngle * 0.5f);

            auto farHeightHalf  = mFarViewDistance * tan;
            auto farWidthHalf   = mViewAspect * farHeightHalf;

            Vec3f right = Vec3f::cross(mWorldDirection, mWorldUp);

            Vec3f far_norm  = -mWorldDirection;
            Vec3f far_point = mWorldPosition + mWorldDirection * mFarViewDistance;

            Vec3f near_norm  = mWorldDirection;
            Vec3f near_point = mWorldPosition + mWorldDirection * mNearViewDistance;

            Vec3f left_far_top = far_point + mWorldUp * farHeightHalf - right * farWidthHalf;
            Vec3f left_far_bottom = left_far_top - mWorldUp * (2.0f * farHeightHalf);
            Vec3f right_far_top = left_far_top + right * (2.0f * farWidthHalf);

            Vec3f left_ftn  = left_far_top.getNormalized();
            Vec3f left_fbn  = left_far_bottom.getNormalized();
            Vec3f right_ftn = right_far_top.getNormalized();

            Vec3f left_norm   = Vec3f::cross(left_ftn, mWorldUp);
            Vec3f top_norm    = Vec3f::cross(left_ftn, right);
            Vec3f bottom_norm = Vec3f::cross(right, left_fbn);
            Vec3f right_norm  = Vec3f::cross(mWorldUp, right_ftn);

            mPlanes[eFS_FRUSTUM_NEAR]   = Plane(near_point,     near_norm  );
            mPlanes[eFS_FRUSTUM_FAR]    = Plane(far_point,      far_norm   );
            mPlanes[eFS_FRUSTUM_BOTTOM] = Plane(mWorldPosition, bottom_norm);
            mPlanes[eFS_FRUSTUM_TOP]    = Plane(mWorldPosition, top_norm   );
            mPlanes[eFS_FRUSTUM_LEFT]   = Plane(mWorldPosition, left_norm  );
            mPlanes[eFS_FRUSTUM_RIGHT]  = Plane(mWorldPosition, right_norm );
        }
    }

} // namespace Berserk::Engine