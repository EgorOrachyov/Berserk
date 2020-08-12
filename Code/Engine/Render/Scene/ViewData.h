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
#include <Scene/Camera.h>
#include <RenderTargets/RenderTarget.h>

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

            /** View (look-at) camera matrix */
            Mat4x4f viewMatrix;

            /** Projection camera matrix (perspective or orthographic) */
            Mat4x4f projMatrix;

            /** In this order projMatrix x viewMatrix  */
            Mat4x4f projViewMatrix;

            /** World space position */
            Vec3f cameraPosition;

            /** World space direction */
            Vec3f cameraDirection;

            /** World space up */
            Vec3f cameraUp;

            /** To know when we use perspective camera */
            ECameraType cameraType = ECameraType::Perspective;

            /** Query and save camera info, which won't be changed among this frame rendering */
            void updateFromCamera(const Camera& camera) {
                viewMatrix = camera.getViewMat();
                projMatrix = camera.getProjMat();
                projViewMatrix = projMatrix * viewMatrix;
                cameraPosition = camera.getPosition();
                cameraDirection = camera.getDirection();
                cameraUp = camera.getUp();
                cameraTransform.getRotation() = camera.getRotation();
                cameraTransform.getOffset() = camera.getPosition();
                cameraType = camera.getType();
            }

            /** Query and save default area settings from target  */
            void updateFromTarget(const RenderTarget& target) {
                viewArea = target.getAreaSize();
                viewRegion = Region2i(0,0,viewArea);
            }

        };

    }
}




#endif //BERSERK_VIEWDATA_H
