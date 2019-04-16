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
            // Recalculate perspective frustum
            // from default properties

            float32 tan = Math::tg(mViewAngle * 0.5f);

            auto farHeightHalf   = mFarViewDistance * tan;
            auto farWidthHalf    = mViewAspect * farHeightHalf;

            auto right           = Vec3f::cross(mWorldDirection, mWorldUp);

            auto far_norm        = -mWorldDirection;
            auto far_point       = mWorldPosition + mWorldDirection * mFarViewDistance;

            auto near_norm       = mWorldDirection;
            auto near_point      = mWorldPosition + mWorldDirection * mNearViewDistance;

            auto left_far_top    = far_point + mWorldUp * farHeightHalf - right * farWidthHalf;
            auto left_far_bottom = left_far_top - mWorldUp * (2.0f * farHeightHalf);
            auto right_far_top   = left_far_top + right * (2.0f * farWidthHalf);

            auto left_ftn        = left_far_top.getNormalized();
            auto left_fbn        = left_far_bottom.getNormalized();
            auto right_ftn       = right_far_top.getNormalized();

            auto left_norm       = Vec3f::cross(left_ftn, mWorldUp);
            auto top_norm        = Vec3f::cross(left_ftn, right);
            auto bottom_norm     = Vec3f::cross(right, left_fbn);
            auto right_norm      = Vec3f::cross(mWorldUp, right_ftn);

            mPlanes[eFS_FRUSTUM_NEAR]   = Plane(near_point,     near_norm  );
            mPlanes[eFS_FRUSTUM_FAR]    = Plane(far_point,      far_norm   );
            mPlanes[eFS_FRUSTUM_BOTTOM] = Plane(mWorldPosition, bottom_norm);
            mPlanes[eFS_FRUSTUM_TOP]    = Plane(mWorldPosition, top_norm   );
            mPlanes[eFS_FRUSTUM_LEFT]   = Plane(mWorldPosition, left_norm  );
            mPlanes[eFS_FRUSTUM_RIGHT]  = Plane(mWorldPosition, right_norm );

            return;
        }
        if (mIsOrthographic)
        {
            // Recalculate orthographic view
            // volume from default properties

            Vec3f right = Vec3f::cross(mWorldDirection, mWorldUp);

            auto near_norm    = mWorldDirection;
            auto near_point   = mWorldPosition + mWorldDirection * mNearViewDistance;

            auto far_norm     = -mWorldDirection;
            auto far_point    = mWorldPosition + mWorldDirection * mFarViewDistance;

            auto bottom_norm  = mWorldUp;
            auto bottom_point = mWorldPosition - mWorldUp * mOrthoViewBottom;

            auto top_norm     = -mWorldUp;
            auto top_point    = mWorldPosition + mWorldUp * mOrthoViewTop;

            auto left_norm    = right;
            auto left_point   = mWorldPosition - right * mOrthoViewLeft;

            auto right_norm   = -right;
            auto right_point  = mWorldPosition + right * mOrthoViewRight;

            mPlanes[eFS_FRUSTUM_NEAR]   = Plane(near_point,   near_norm  );
            mPlanes[eFS_FRUSTUM_FAR]    = Plane(far_point,    far_norm   );
            mPlanes[eFS_FRUSTUM_BOTTOM] = Plane(bottom_point, bottom_norm);
            mPlanes[eFS_FRUSTUM_TOP]    = Plane(top_point,    top_norm   );
            mPlanes[eFS_FRUSTUM_LEFT]   = Plane(left_point,   left_norm  );
            mPlanes[eFS_FRUSTUM_RIGHT]  = Plane(right_point,  right_norm );

            return;
        }
    }

} // namespace Berserk::Engine