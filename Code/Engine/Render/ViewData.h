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
#include <Math/Quatf.h>
#include <Math/Region2i.h>
#include <Math/Transformf.h>

namespace Berserk {
    namespace Render {

        /** Final view settings */
        class ViewData {
        public:

            /** Final view area, which will be set up as a viewport for the rendering */
            Region2i viewRegion;

            /** Final viewport (width,height) */
            Size2i viewArea;

            /** Camera transformation if required */
            Transformf cameraTransform;

            /** */
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
