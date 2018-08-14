//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_CAMERA_H
#define BERSERKENGINE_CAMERA_H

#include "Objects/Actor.h"
#include "Components/CameraComponent.h"

#include "Essential/UsageDescriptors.h"

namespace Berserk
{

    class ENGINE_EXPORT Camera : public Actor
    {
    public:

        Camera(const CStaticString &name, FLOAT32 lifeTime = 0);
        virtual ~Camera() = default;

        void setPosition(const Vector3f &position);
        void setDirection(const Vector3f &direction);
        void setOrientation(const Vector3f &orientation);

        const Vector3f& getPosition() const;
        const Vector3f& getDirection() const;
        const Vector3f& getOrientation() const;

        void setNearClipDistance(FLOAT32 near);
        void setFarClipDistance(FLOAT32 far);

        void setViewAngle(FLOAT32 angle);
        void setAspectRatio(FLOAT32 aspect);
        void setAutoAspectRatio(bool setIn = true);

        void setViewSpace(FLOAT32 left, FLOAT32 right, FLOAT32 bottom, FLOAT32 top);
        void setViewSpace(FLOAT32 width, FLOAT32 height);
        void getViewSpace(FLOAT32& width, FLOAT32& height) const;

        void setCinematicViewport(bool cinematic = true);
        void setCinematicBorder(UINT32 border);
        void setViewport(UINT32 posX, UINT32 posY, UINT32 width, UINT32 height);
        void getViewport(UINT32& posX, UINT32& posY, UINT32& width, UINT32& height);

        void setPerspectiveView();
        void setOrthographicView();
        void setSymmetricOrthoView(bool setIn = true);

        bool isAutoAspectRatio() const;
        bool isCinematicViewport() const;
        bool isPerspective() const;
        bool isOrthographic() const;
        bool isSymmetricOrthoView() const;

        UINT32 getCinematicBorder() const;

        CameraComponent* getComponent();

    protected:

        void process(FLOAT64 delta, const Matrix4x4f &rootTransformation) override;

    private:

        INT8 mIsAutoAspectRatio     : 1;
        INT8 mIsPerspectiveView     : 1;
        INT8 mIsOrthographicView    : 1;
        INT8 mIsCinematicViewport   : 1;
        INT8 mIsSymmetricOrthoView  : 1;

        UINT32 mCinematicBorder;

        Vector3f mPosition;
        Vector3f mDirection;
        Vector3f mOrientation;

        FLOAT32 mNearClipDistance;
        FLOAT32 mFarClipDistance;
        FLOAT32 mAngle;
        FLOAT32 mAspect;

        FLOAT32 mWidth;
        FLOAT32 mHeight;
        FLOAT32 mLeft;
        FLOAT32 mRight;
        FLOAT32 mBottom;
        FLOAT32 mTop;

        CameraComponent mCameraComponent;
    };

} // namespace Berserk

#endif //BERSERKENGINE_CAMERA_H