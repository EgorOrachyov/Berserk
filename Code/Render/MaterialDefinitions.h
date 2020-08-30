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

        /** Defines shading algorithm, used for the material */
        enum class EMaterialShading : uint8 {
            Unlit = 0,
            BlinnPhong = 1,
            PBR = 2,
            Max = 3
        };

        /** Defines how to transform vertex input position */
        enum class EMaterialDomain : uint8 {
            Model
        };

    }
}

#endif //BERSERK_MATERIALDEFINITION_H
