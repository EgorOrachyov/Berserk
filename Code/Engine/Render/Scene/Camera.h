/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CAMERA_H
#define BERSERK_CAMERA_H

#include <Math/Vec3f.h>
#include <Math/Mat4x4f.h>
#include <Math/Quatf.h>

namespace Berserk {
    namespace Render {

        /** Camera projection type */
        enum class ECameraType {
            Perspective,
            Orthogonal
        };

        /**
         * @brief Virtual 3D space camera (view)
         *
         * By default camera is created with direction vector (0,0,-1)
         * an up vector (0,1,0) in 3D right-handed space.
         *
         *      |y
         *      |
         *      |_____x
         *     /
         *    /
         *   z
         *
         * Camera rotations will rotate pair dir / up, what will determine
         * the view matrix of the camera for the scene.
         */
        class Camera {
        public:

            /**
             * Constructs perspective camera
             * @param fovRad View angle between top and down planes
             * @param aspect W / H ration of the view frustum
             * @param near Near view plane
             * @param far Far view plane
             */
            Camera(float fovRad, float aspect, float near, float far);

            /**
            * Constructs perspective camera with auto aspect evaluated (from rendering target settings)
            * @param fovRad View angle between top and down planes
            * @param near Near view plane
            * @param far Far view plane
            */
            Camera(float fovRad, float near, float far);

            ECameraType getType() const { return mType; }

            float getVerticalFov() const { return mFov; }

            float getAspectWH() const { return mAspect; }

            float getNearZ() const { return mNearZ; }

            float getFarZ() const { return mFarZ; }

            float getLeft() const { return mLeft; }

            float getRight() const { return mRight; }

            bool getIsAutoAspect() const { return mAutoAspect; }

            const Vec3f& getPosition() const { return mPosition; }

            const Quatf& getRotation() const { return mRotation; }

            /** @return Evaluated view matrix with current camera setting */
            Mat4x4f getViewMat() const;

            /** @return Evaluated projection matrix with current camera setting */
            Mat4x4f getProjMat() const;

            /** @return Camera Direction vector with current rotation settings */
            Vec3f getDirection() const;

            /** @return Camera Up vector with current rotation settings */
            Vec3f getUp() const;

            /** Set camera aspect (used for dynamic auto aspect cameras)*/
            void setAspect(float aspect) { mAspect = aspect; }

            /** Moves camera in t direction */
            void move(const Vec3f& t) { mPosition += t; }

        private:
            ECameraType mType = ECameraType::Perspective;
            float mFov = Math::HALF_PIf;
            float mAspect = 1.0f;
            float mNearZ = 0.01f;
            float mFarZ = 100.0f;
            float mLeft = 0.0f;
            float mRight = 1.0f;
            bool mAutoAspect = true;

            /** World space position */
            Vec3f mPosition;
            /** Rotation of the camera Dir/Up space */
            Quatf mRotation;
        };

    }
}

#endif //BERSERK_CAMERA_H