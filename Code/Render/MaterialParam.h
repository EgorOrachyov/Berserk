/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MATERIALPARAM_H
#define BERSERK_MATERIALPARAM_H

#include <RHI/RHIDefinitions.h>

namespace Berserk {
    namespace Render {

        /** Generic material param, available in material shader */
        class MaterialParam {
        public:
            /** Param name (the same as in the shader, without spaces) */
            CString name;

            /** Param type */
            EShaderData dataType = EShaderData::Unknown;

            /** True if this param is texture (stored out of uniform block) */
            bool isTexture = false;
        };

    }
}

#endif //BERSERK_MATERIALPARAM_H