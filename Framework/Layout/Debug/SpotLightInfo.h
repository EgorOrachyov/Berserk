//
// Created by Egor Orachyov on 17.07.2018.
//

#ifndef BERSERKENGINE_GLSPOTLIGHT_H
#define BERSERKENGINE_GLSPOTLIGHT_H

#include "Essential/Types.h"
#include "Math/Vector3f.h"

namespace Berserk
{

    struct SpotLightInfo
    {
    public:

        Vector3f mWorldPosition;
        Vector3f mDirection;
        Vector3f mLightIntensity;

        FLOAT32 mAngleCosine;
        FLOAT32 mAttenuationExponent;

        INT8 mCreatesShadows;
    };

}

#endif //BERSERKENGINE_GLSPOTLIGHT_H
