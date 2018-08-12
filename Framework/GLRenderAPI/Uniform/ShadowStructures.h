//
// Created by Egor Orachyov on 12.08.2018.
//

#ifndef BERSERKENGINE_SHADOWSTRUCTURES_H
#define BERSERKENGINE_SHADOWSTRUCTURES_H

#include "Essential/Types.h"
#include "Render/RenderSystem.h"

namespace Berserk
{

    struct UShadowSpotLight
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

    struct UShadowPointLight
    {
    public:
        INT32 Position;
        INT32 Intensity;

        INT32 constant;
        INT32 linear;
        INT32 quadratic;

        INT32 radius;
    };

    struct UShadowDirectLight
    {
    public:
        INT32 Direction;
        INT32 Intensity;

        INT32 map;
    };

    class UShadowLightsInfo
    {
    public:

        INT32 NUM_DIR_SHADOWS;

        INT32 NUM_OF_DIR_SHADOW_LIGHTS;
        //INT32 NUM_OF_SPOT_SHADOW_LIGHTS;
        //INT32 NUM_OF_POINT_SHADOW_LIGHTS;

        INT32 light_MVP_dir[ShadowInfo::SI_MAX_DIRECTIONAL_SHADOWS];

        //UShadowSpotLight spotLights[ShadowInfo::SI_MAX_DIRECTIONAL_SHADOWS];
        //UShadowPointLight pointLights[ShadowInfo::SI_MAX_OMNIDIRECTIONAL_SHADOWS];
        UShadowDirectLight dirShadowLights[ShadowInfo::SI_MAX_DIRECTIONAL_SHADOWS];
    };

} // namespace Berserk

#endif //BERSERKENGINE_SHADOWSTRUCTURES_H
