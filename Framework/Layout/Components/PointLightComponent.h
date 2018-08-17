//
// Created by Egor Orachyov on 27.07.2018.
//

#ifndef BERSERKENGINE_POINTLIGHTCOMPONENT_H
#define BERSERKENGINE_POINTLIGHTCOMPONENT_H

#include "Math/Vector3f.h"
#include "Math/Vector4f.h"

namespace Berserk
{

    class PointLightComponent
    {
    public:

        Vector3f mLightIntensity;
        Vector4f mPosition;

        FLOAT32 mRadius;
        FLOAT32 mConstantAttenuation;
        FLOAT32 mLinearAttenuation;
        FLOAT32 mQuadraticAttenuation;
    };

} // namespace Berserk

#endif //BERSERKENGINE_POINTLIGHTCOMPONENT_H