/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIDESCS_H
#define BERSERK_RHIDESCS_H

#include <TArray.h>
#include <TArrayStatic.h>
#include <RHI/RHIResources.h>

namespace Berserk {

    struct RHIVertexElement {
        uint32 offset;
        uint32 stride;
        uint32 buffer;
        uint32 location;
        EVertexElementType type;
        EVertexIterating iterating;

        bool operator==(const RHIVertexElement& other) const {
            return offset    == other.offset     &&
                   stride    == other.stride     &&
                   buffer    == other.buffer     &&
                   location  == other.location   &&
                   type      == other.type       &&
                   iterating == other.iterating;
        }
    };

    using RHIVertexDeclarationDesc = TArray<RHIVertexElement>;

    struct RHIShaderModuleDesc {
        EShaderType type;
        TArray<uint8> code;
    };

    using RHIShaderDesc = TArrayStatic<RHIShaderModuleDesc>;

    struct RHISamplerDesc {
        ESamplerFilter min;
        ESamplerFilter mag;
        ESamplerFilter mipmapMode;
        ESamplerRepeatMode u;
        ESamplerRepeatMode v;
        ESamplerRepeatMode w;
        ESamplerBorderColor color;
        bool useAnisotropy;

        bool operator==(const RHISamplerDesc& other) const {
            return min             == other.min            &&
                   mag             == other.mag            &&
                   mipmapMode      == other.mipmapMode     &&
                   u               == other.u              &&
                   v               == other.v              &&
                   w               == other.w              &&
                   color           == other.color          &&
                   useAnisotropy   == other.useAnisotropy;
        }
    };

    struct RHIUniformTextureDesc {
        int32 binding;
        TPtrShared<RHITexture> texture;
        TPtrShared<RHISampler> sampler;
    };

    struct RHIUniformBufferDesc {
        int32 binding;
        uint32 range;
        uint32 offset;
        TPtrShared<RHIUniformBuffer> buffer;
    };

}

#endif //BERSERK_RHIDESCS_H
