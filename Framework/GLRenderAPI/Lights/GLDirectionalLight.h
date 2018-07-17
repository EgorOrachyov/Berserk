//
// Created by Egor Orachyov on 17.07.2018.
//

#ifndef BERSERKENGINE_GLDIRECTIONALLIGHT_H
#define BERSERKENGINE_GLDIRECTIONALLIGHT_H

#include "Essential/Types.h"
#include "Math/Vector3f.h"

namespace Berserk
{

    struct GLDirectionalLight
    {
    public:

        Vector3f mFrontDirection;
        Vector3f mUpDirection;
        Vector3f mLightIntensity;

        INT8 mCreatesShadows;
    };

}

#endif //BERSERKENGINE_GLDIRECTIONALLIGHT_H