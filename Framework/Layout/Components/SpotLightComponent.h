//
// Created by Egor Orachyov on 27.07.2018.
//

#ifndef BERSERKENGINE_SPOTLIGHTCOMPONENT_H
#define BERSERKENGINE_SPOTLIGHTCOMPONENT_H

#include "Math/Vector3f.h"
#include "Math/Vector4f.h"

namespace Berserk
{

    class SpotLightComponent
    {
    public:

        Vector3f mLightIntensity;
        Vector4f mDirection;
        Vector4f mPosition;

        FLOAT32 mCutoff;
        FLOAT32 mOuterCutoff;
        FLOAT32 mEpsilon;
        FLOAT32 mAttenuationExponent;

        INT8 mCastShadows : 1;
    };

} // namespace Berserk

#endif //BERSERKENGINE_SPOTLIGHTCOMPONENT_H