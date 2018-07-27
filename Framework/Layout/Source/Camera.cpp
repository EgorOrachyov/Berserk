//
// Created by Egor Orachyov on 26.07.2018.
//

#include "Objects/Cameras/Camera.h"
#include "Math/UtilityMatrices.h"
#include "Managers/SceneManager.h"

namespace Berserk
{

    Camera::Camera(const CStaticString &name, FLOAT32 lifeTime) : Actor(name, lifeTime)
    {
        mIsAutoAspectRatio = true;
        mIsPerspectiveView = true;
        mIsOrthographicView = false;

        mPosition = Vector3f(0,0,0);
        mDirection = Vector3f(0,0,1);
        mOrientation = Vector3f(0,1,0);

        mNearClipDistance = 1;
        mFarClipDistance = 16;
        mAngle = 0.5;
        mAspect = 1;

        mWidth = 2;
        mHeight = 2;
        mLeft = -1;
        mRight = 1;
        mBottom = -1;
        mTop = 1;
    }

    void Camera::setPosition(const Vector3f &position)
    {
        if (mIsEditable)
        {
            mPosition = position;
        }
    }

    void Camera::setDirection(const Vector3f &direction)
    {
        ASSERT(!(direction == Vector3f(0,0,0)), "Camera direction should not be 0 vector");

        if (mIsEditable)
        {
            mDirection = direction;
        }
    }

    void Camera::setOrientation(const Vector3f &orientation)
    {
        ASSERT(!(orientation == Vector3f(0,0,0)), "Camera orientation should not be 0 vector");

        if (mIsEditable)
        {
            mOrientation = orientation;
        }
    }

    void Camera::setNearClipDistance(FLOAT32 near)
    {
        ASSERT(near > 0, "Near clip distance should be more than 0");

        if (mIsEditable)
        {
            mNearClipDistance = near;
        }
    }

    void Camera::setFarClipDistance(FLOAT32 far)
    {
        ASSERT(far > 0, "Far clip distance should be more than 0");

        if (mIsEditable)
        {
            mFarClipDistance = far;
        }
    }

    void Camera::setViewAngle(FLOAT32 angle)
    {
        if (mIsEditable)
        {
            mAngle = angle;
        }
    }

    void Camera::setAspectRatio(FLOAT32 aspect)
    {
        if (mIsEditable)
        {
            mAspect = aspect;
        }
    }

    void Camera::setAutoAspectRatio(bool setIn)
    {
        if (mIsEditable)
        {
            mIsAutoAspectRatio = setIn;
        }
    }

    void Camera::setViewSpace(FLOAT32 left, FLOAT32 right, FLOAT32 bottom, FLOAT32 top)
    {
        if (mIsEditable)
        {
            mLeft = left;
            mRight = right;
            mBottom = bottom;
            mTop = top;
        }
    }

    void Camera::setViewSpace(FLOAT32 width, FLOAT32 height)
    {
        if (mIsEditable)
        {
            mWidth = width;
            mHeight = height;
        }
    }

    void Camera::setPerspectiveView()
    {
        if (mIsEditable)
        {
            mIsOrthographicView = false;
            mIsPerspectiveView = true;
        }
    }

    void Camera::setOrthographicView()
    {
        if (mIsEditable)
        {
            mIsPerspectiveView = false;
            mIsOrthographicView = true;
        }
    }

    bool Camera::isAutoAspectRatio() const
    {
        return mIsAutoAspectRatio;
    }

    bool Camera::isPerspective() const
    {
        return mIsPerspectiveView;
    }

    bool Camera::isOrthographic() const
    {
        return mIsOrthographicView;
    }

    void Camera::process(FLOAT64 delta, const Matrix4x4f &rootTransformation)
    {
        Actor::process(delta, rootTransformation);

        if (mIsActive)
        {
            Matrix4x4f transf = rootTransformation * mTransformation;

            Vector4f pos = transf * Vector4f(mPosition.x, mPosition.y, mPosition.z, 1);
            Vector4f dir = transf * Vector4f(mDirection.x, mDirection.y, mDirection.z, 0);
            Vector4f orient = transf * Vector4f(mOrientation.x, mOrientation.y, mOrientation.z, 0);

            Vector3f eye = Vector3f(pos.x, pos.y, pos.z);
            Vector3f at = eye + Vector3f(dir.x, dir.y, dir.z);
            Vector3f up = Vector3f(orient.x, orient.y, orient.z);

            mCameraComponent.mView = lookAt(eye, at, up);

            if (mIsPerspectiveView)
            {
                mCameraComponent.mProjection = perspective(mAngle, mAspect, mNearClipDistance, mFarClipDistance);
            }
            else
            {
                mCameraComponent.mProjection = orthographic(mLeft, mRight, mBottom, mTop, mNearClipDistance, mFarClipDistance);
            }

            gSceneManager->getRenderManager().queueCamera(&mCameraComponent);
        }
    }

} // namespace Berserk