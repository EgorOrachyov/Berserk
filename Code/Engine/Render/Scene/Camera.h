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

        enum class ECameraType {
            Perspective,
            Orthogonal
        };

        class Camera {
        public:


        private:
            ECameraType mType = ECameraType::Perspective;
            float mFov;
            float mAspect;
            float mNearZ;
            float mFarZ;

            Vec3f mPosition;
            Quatf mRotation;
        };

    }
}

#endif //BERSERK_CAMERA_H