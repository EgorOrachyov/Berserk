//
// Created by Egor Orachyov on 27.07.2018.
//

#ifndef BERSERKENGINE_CAMERACOMPONENT_H
#define BERSERKENGINE_CAMERACOMPONENT_H

#include "Math/Matrix4x4f.h"

namespace Berserk
{

    class CameraComponent
    {
    public:

        struct Viewport
        {
        public:

            UINT32 posX;
            UINT32 posY;
            UINT32 width;
            UINT32 height;

        };

        Matrix4x4f mView;
        Matrix4x4f mProjection;
        Viewport   mViewport;
    };

} // namespace Berserk

#endif //BERSERKENGINE_CAMERACOMPONENT_H