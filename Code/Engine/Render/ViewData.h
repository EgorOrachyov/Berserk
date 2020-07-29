/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VIEWDATA_H
#define BERSERK_VIEWDATA_H

#include <Math/Mat4x4f.h>
#include <Math/Vec3f.h>
#include <Math/Region2i.h>

namespace Berserk {
    namespace Render {

        /** Final view settings */
        class ViewData {
        public:

            Region2i viewArea;

            Mat4x4f cameraWorldRotation;

            Mat4x4f viewMatrix;

            Mat4x4f projectionMatrix;

            Mat4x4f projectionViewMatrix;

            Vec3f cameraPosition;

            Vec3f cameraDirection;

            Vec3f cameraUp;


        };

    }
}




#endif //BERSERK_VIEWDATA_H
