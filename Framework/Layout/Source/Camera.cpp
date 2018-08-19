//
// Created by Egor Orachyov on 26.07.2018.
//

#include <Render/RenderSystem.h>
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
        mIsCinematicViewport = false;

        mCinematicBorder = 0;

        mWorldPosition = Vector3f(0);
        mPosition = Vector3f(0);
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

        mCameraComponent.mViewport.posX = 0;
        mCameraComponent.mViewport.posY = 0;
        mCameraComponent.mViewport.width = 640;
        mCameraComponent.mViewport.height = 640;
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
        if (mIsEditable)
        {
            mDirection = direction;
        }
    }

    void Camera::setOrientation(const Vector3f &orientation)
    {
        if (mIsEditable)
        {
            mOrientation = orientation;
        }
    }

    const Vector3f& Camera::getWorldPosition() const
    {
        return mWorldPosition;
    }

    const Vector3f& Camera::getPosition() const
    {
        return mPosition;
    }

    const Vector3f& Camera::getDirection() const
    {
        return mDirection;
    }

    const Vector3f& Camera::getOrientation() const
    {
        return mOrientation;
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

            if (mIsSymmetricOrthoView)
            {
                mLeft = -width * 0.5;
                mRight = width * 0.5;
                mBottom = -height * 0.5;
                mTop = height * 0.5;
            }
        }
    }

    void Camera::getViewSpace(FLOAT32 &width, FLOAT32 &height) const
    {
        width = mWidth;
        height = mHeight;
    }

    void Camera::setCinematicViewport(bool cinematic)
    {
        mIsCinematicViewport = cinematic;
    }

    void Camera::setCinematicBorder(UINT32 border)
    {
        mCinematicBorder = border;
    }

    void Camera::setViewport(UINT32 posX, UINT32 posY, UINT32 width, UINT32 height)
    {
        if (mIsCinematicViewport)
        {
            mCameraComponent.mViewport.posX = posX;
            mCameraComponent.mViewport.posY = posY + mCinematicBorder;
            mCameraComponent.mViewport.width = width;
            mCameraComponent.mViewport.height = height - 2 * mCinematicBorder;

            if (mIsAutoAspectRatio)
            {
                setAspectRatio((FLOAT32)width / (FLOAT32)(height - 2 * mCinematicBorder));
            }
        }
        else
        {
            mCameraComponent.mViewport.posX = posX;
            mCameraComponent.mViewport.posY = posY;
            mCameraComponent.mViewport.width = width;
            mCameraComponent.mViewport.height = height;

            if (mIsAutoAspectRatio)
            {
                setAspectRatio((FLOAT32)width / (FLOAT32)height);
            }
        }
    }

    void Camera::getViewport(UINT32& posX, UINT32& posY, UINT32& width, UINT32& height)
    {
        posX = mCameraComponent.mViewport.posX;
        posY = mCameraComponent.mViewport.posY;
        width = mCameraComponent.mViewport.width;
        height = mCameraComponent.mViewport.height;
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

    void Camera::setSymmetricOrthoView(bool setIn)
    {
        if (mIsEditable)
        {
            mIsSymmetricOrthoView = setIn;
        }
    }

    bool Camera::isAutoAspectRatio() const
    {
        return mIsAutoAspectRatio;
    }

    bool Camera::isCinematicViewport() const
    {
        return mIsCinematicViewport;
    }

    bool Camera::isPerspective() const
    {
        return mIsPerspectiveView;
    }

    bool Camera::isOrthographic() const
    {
        return mIsOrthographicView;
    }

    bool Camera::isSymmetricOrthoView() const
    {
        return mIsSymmetricOrthoView;
    }

    UINT32 Camera::getCinematicBorder() const
    {
        return mCinematicBorder;
    }

    CameraComponent* Camera::getComponent()
    {
        return &mCameraComponent;
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
            mWorldPosition = Vector3f(pos);

            if (mIsPerspectiveView)
            {
                mCameraComponent.mProjection = perspective(mAngle, mAspect, mNearClipDistance, mFarClipDistance);
            }
            else
            {
                mCameraComponent.mProjection = orthographic(mLeft, mRight, mBottom, mTop, mNearClipDistance, mFarClipDistance);
            }

            gRenderSystem->setRenderCamera(this);
        }
    }

} // namespace Berserk