//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_CAMERA_H
#define BERSERKENGINE_CAMERA_H

#include "Objects/Actor.h"

namespace Berserk
{

    class Camera : public Actor
    {
    public:

        Camera(const CStaticString &name, FLOAT32 lifeTime = 0);
        virtual ~Camera() = default;

        void setPosition(const Vector3f &position);
        void setDirection(const Vector3f &direction);
        void setOrientation(const Vector3f &orientation);

        void setNearClipDistance(FLOAT32 near);
        void setFarClipDistance(FLOAT32 far);

        void setViewAngle(FLOAT32 angle);
        void setAspectRatio(FLOAT32 aspect);
        void setAutoAspectRatio(bool setIn = true);
        void setSetViewSpace(FLOAT32 left, FLOAT32 right, FLOAT32 bottom, FLOAT32 top);

        void setPerspectiveView();
        void setOrthographicView();

        bool isAutoAspectRatio() const;
        bool isPerspective() const;
        bool isOrthographic() const;

    private:

        INT8 mIsAutoAspectRatio : 1;
        INT8 mIsPerspectiveView : 1;
        INT8 mIsOrthographicView : 1;

        Vector3f mPosition;
        Vector3f mDirection;
        Vector3f mOrientation;

        FLOAT32 mNearClipDistance;
        FLOAT32 mFarClipDistance;
        FLOAT32 mAngle;
        FLOAT32 mAspect;
        FLOAT32 mLeft;
        FLOAT32 mRight;
        FLOAT32 mBottom;
        FLOAT32 mTop;

        Matrix4x4f mView;
        Matrix4x4f mProjection;

    };

} // namespace Berserk

#endif //BERSERKENGINE_CAMERA_H