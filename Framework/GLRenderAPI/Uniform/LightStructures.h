//
// Created by Egor Orachyov on 01.08.2018.
//

#ifndef BERSERKENGINE_LIGHTSTRUCTURES_H
#define BERSERKENGINE_LIGHTSTRUCTURES_H

#include "Misc/Types.h"
#include "Render/RenderSystem.h"

namespace Berserk
{

    struct USpotLight
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

    struct UPointLight
    {
    public:
        INT32 Position;
        INT32 Intensity;

        INT32 constant;
        INT32 linear;
        INT32 quadratic;

        INT32 radius;
    };

    struct UDirectLight
    {
    public:
        INT32 Direction;
        INT32 Intensity;
    };

    class ULightsInfo
    {
    public:
        INT32 NUM_OF_DIR_LIGHTS;
        INT32 NUM_OF_SPOT_LIGHTS;
        INT32 NUM_OF_POINT_LIGHTS;

        USpotLight spotLights[LightInfo::LI_MAX_SPOT_LIGHTS];
        UPointLight pointLights[LightInfo::LI_MAX_POINT_LIGHTS];
        UDirectLight directLights[LightInfo::LI_MAX_DIRECTIONAL_LIGHTS];

        INT32 ambientLight;
    };

} // namespace Berserk

#endif //BERSERKENGINE_LIGHTSTRUCTURES_H
