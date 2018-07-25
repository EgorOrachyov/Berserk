//
// Created by Egor Orachyov on 17.07.2018.
//

#ifndef BERSERKENGINE_GLPOINTLIGHT_H
#define BERSERKENGINE_GLPOINTLIGHT_H

#include "Essential/Types.h"
#include "Math/Vector3f.h"

namespace Berserk
{

    struct PointLightInfo
    {
    public:

        Vector3f mWorldPosition;
        Vector3f mLightIntensity;

        FLOAT32 mConstantAttenuation;
        FLOAT32 mLinearAttenuation;
        FLOAT32 mQuadraticAttenuation;

        INT8 mCreatesShadows;
    };

}

#endif //BERSERKENGINE_GLPOINTLIGHT_H
