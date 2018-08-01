//
// Created by Egor Orachyov on 01.08.2018.
//

#ifndef BERSERKENGINE_LIGHTSTRUCTURES_H
#define BERSERKENGINE_LIGHTSTRUCTURES_H

#include "Essential/Types.h"
#include "Managers/RenderManager.h"

namespace Berserk
{

    struct SpotLight
    {
    public:
        INT32 Position;
        INT32 Direction;
        INT32 Intensity;

        INT32 cutoff;
        INT32 outerCutoff;
        INT32 epsilon;
        INT32 exponent;
    };

    struct PointLight
    {
    public:
        INT32 Position;
        INT32 Intensity;

        INT32 constant;
        INT32 linear;
        INT32 quadratic;

        INT32 radius;
    };

    struct DirectLight
    {
    public:
        INT32 Direction;
        INT32 Intensity;
    };

    class LightsInfo
    {
    public:
        INT32 NUM_OF_DIR_LIGHTS;
        INT32 NUM_OF_SPOT_LIGHTS;
        INT32 NUM_OF_POINT_LIGHTS;

        SpotLight spotLights[LightInfo::LI_MAX_SPOT_LIGHTS];
        PointLight pointLights[LightInfo::LI_MAX_POINT_LIGHTS];
        DirectLight directLights[LightInfo::LI_MAX_DIRECTIONAL_LIGHTS];

        INT32 ambientLight;
    };

} // namespace Berserk

#endif //BERSERKENGINE_LIGHTSTRUCTURES_H
