/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Scene/Camera.h>

namespace Berserk {
    namespace Render {

        Camera::Camera(float fovRad, float near, float far) {
            mFov = fovRad;
            mAutoAspect = true;
            mNearZ = near;
            mFarZ = far;
        }

        Camera::Camera(float fovRad, float aspect, float near, float far) {
            mFov = fovRad;
            mAspect = aspect;
            mNearZ = near;
            mFarZ = far;
        }

        Mat4x4f Camera::getViewMat() const {
            Vec3f dir(0,0,-1);
            Vec3f up(0,1,0);

            dir = mRotation.rotate(dir);
            up = mRotation.rotate(up);

            return Mat4x4f::lookAt(mPosition, dir, up);
        }

        Mat4x4f Camera::getProjMat() const {
            return Mat4x4f::perspective(mFov, mAspect, mNearZ, mFarZ);
        }

    }
}