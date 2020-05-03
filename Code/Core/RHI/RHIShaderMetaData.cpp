/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RHI/RHIShaderMetaData.h>
#include "RHIDefinitions.h"

namespace Berserk {

    const char * RHIShaderMetaData::getShaderDataName(EShaderData dataType) {
        switch (dataType) {
            case EShaderData::Float1:
                return "Float1";
            case EShaderData::Float2:
                return "Float2";
            case EShaderData::Float3:
                return "Float3";
            case EShaderData::Float4:
                return "Float4";
            case EShaderData::Mat2:
                return "Mat2";
            case EShaderData::Mat3:
                return "Mat3";
            case EShaderData::Mat4:
                return "Mat4";
            case EShaderData::Sampler2D:
                return "Sampler2D";
            case EShaderData::Sampler3D:
                return "Sampler3D";
            case EShaderData::SamplerCube:
                return "SamplerCube";
            default:
                return "Undefined";
        }
    }

}