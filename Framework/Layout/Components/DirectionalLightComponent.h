//
// Created by Egor Orachyov on 27.07.2018.
//

#ifndef BERSERKENGINE_DIRECTIONALLIGHTCOMPONENT_H
#define BERSERKENGINE_DIRECTIONALLIGHTCOMPONENT_H

#include "Math/Vector3f.h"
#include "Math/Vector4f.h"

namespace Berserk
{

    class DirectionalLightComponent
    {
    public:

        Vector3f mLightIntensity;
        Vector4f mDirection;

        INT8 mCastShadows : 1;
    };

} // namespace Berserk

#endif //BERSERKENGINE_DIRECTIONALLIGHTCOMPONENT_H