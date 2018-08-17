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

        INT32 map;
    };

    struct UShadowPointLight
    {
    public:
        INT32 WorldPos;
        INT32 Position;
        INT32 Intensity;

        INT32 constant;
        INT32 linear;
        INT32 quadratic;

        INT32 radius;

        INT32 map;
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
        INT32 NUM_SPOT_SHADOWS;
        INT32 NUM_POINT_SHADOWS;

        INT32 NUM_OF_DIR_SHADOW_LIGHTS;
        INT32 NUM_OF_SPOT_SHADOW_LIGHTS;

        INT32 light_PV_dir[ShadowInfo::SI_MAX_DIR_SHADOW_SOURCES];
        INT32 light_PV_spot[ShadowInfo::SI_MAX_SPOT_SHADOW_SOURCES];

        UShadowDirectLight dirSLights[ShadowInfo::SI_MAX_DIR_SHADOW_SOURCES];
        UShadowSpotLight spotSLights[ShadowInfo::SI_MAX_SPOT_SHADOW_SOURCES];
        UShadowPointLight pointSLights[ShadowInfo::SI_MAX_POINT_SHADOW_SOURCES];
    };

} // namespace Berserk

#endif //BERSERKENGINE_SHADOWSTRUCTURES_H
