//
// Created by Egor Orachyov on 18.07.2018.
//

#ifndef BERSERKENGINE_CAMERAINFO_H
#define BERSERKENGINE_CAMERAINFO_H

#include "Math/Matrix4x4f.h"

namespace Berserk
{

    struct CameraInfo
    {
    public:

        Vector4f   mViewport;
        Matrix4x4f mView;
        Matrix4x4f mPerspective;

    };

}

#endif //BERSERKENGINE_CAMERAINFO_H
