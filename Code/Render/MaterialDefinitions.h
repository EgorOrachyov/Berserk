/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MATERIALDEFINITION_H
#define BERSERK_MATERIALDEFINITION_H

#include <Typedefs.h>

namespace Berserk {
    namespace Render {

        /** Material physical type, critical for performance */
        enum class EMaterialBlending : uint8 {
            Opaque = 0,
            Transparent = 1
        };

        /** Textures, provided by material */
        enum class EMaterialTexture : uint8 {
            Albedo = 0,
            Specular = 1,
            Metallic = 2,
            Roughness = 3,
            Normal = 4,
            Displacement = 5,
            AmbientOcclusion = 6,
            Emission = 7,
            Max = 8
        };

        /** Defines shading algorithm, used for the material */
        enum class EMaterialShading : uint8 {
            Unlit = 0,
            BlinnPhong = 1,
            PBR = 2,
            Custom = 3,
            Max = 4
        };

    }
}

#endif //BERSERK_MATERIALDEFINITION_H
