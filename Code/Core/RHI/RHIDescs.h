/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIDESCS_H
#define BERSERK_RHIDESCS_H

#include <TPtrShared.h>
#include <PixelFormat.h>
#include <Containers/TArray.h>
#include <Containers/TArrayStatic.h>
#include <RHI/RHIDefinitions.h>

namespace Berserk {

    struct RHICapabilities {
        int32 UNIFORM_BUFFER_OFFSET_ALIGNMENT = 0;
    };

    struct RHIVertexElement {
        uint32 offset;
        uint32 stride;
        uint8 buffer;
        uint8 location;
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

        Crc32::Builder& buildHash(Crc32::Builder &builder) const {
            builder.hashRaw(&offset, sizeof(offset));
            builder.hashRaw(&stride, sizeof(stride));
            builder.hashRaw(&buffer, sizeof(buffer));
            builder.hashRaw(&location, sizeof(location));
            builder.hashRaw(&type, sizeof(type));
            builder.hashRaw(&iterating, sizeof(iterating));

            return builder;
        }

        uint32 hash() const {
            Crc32::Builder builder;
            return buildHash(builder).getHash();
        }

        friend Archive& operator<<(Archive& archive, const RHIVertexElement& element) {
            archive << element.offset;
            archive << element.stride;
            archive << element.buffer;
            archive << element.location;
            archive << (uint32) element.type;
            archive << (uint32) element.iterating;

            return archive;
        }

        friend Archive& operator>>(Archive& archive, RHIVertexElement& element) {
            archive >> element.offset;
            archive >> element.stride;
            archive >> element.buffer;
            archive >> element.location;
            archive >> (uint32&) element.type;
            archive >> (uint32&) element.iterating;

            return archive;
        }
    };

    using RHIVertexDeclarationDesc = TArrayStatic<RHIVertexElement,RHIConst::MAX_VERTEX_ATTRIBUTES>;

    struct RHIShaderModuleDesc {
        EShaderType type;
        TArray<uint8> code;
    };

    struct RHIShaderModuleViewDesc {
        EShaderType type;
        const TArray<uint8> *code;
    };

    using RHIShaderDesc = TArrayStatic<RHIShaderModuleDesc>;
    using RHIShaderViewDesc = TArrayStatic<RHIShaderModuleViewDesc>;

    struct RHISamplerDesc {
        static const int8 MIN_LOD = -100;
        static const int8 MAX_LOD = 100;

        int8 minLod;
        int8 maxLod;
        bool useMips : 1;
        ESamplerFilter min : 2;
        ESamplerFilter mag : 2;
        ESamplerFilter mipmapMode: 2;
        ESamplerRepeatMode u : 4;
        ESamplerRepeatMode v : 4;
        ESamplerRepeatMode w : 4;
        ESamplerBorderColor color : 4;

        RHISamplerDesc() {
            min = ESamplerFilter::Linear;
            mag = ESamplerFilter::Linear;
            mipmapMode = ESamplerFilter::Linear;
            u = ESamplerRepeatMode::Repeat;
            v = ESamplerRepeatMode::Repeat;
            w = ESamplerRepeatMode::Repeat;
            color = ESamplerBorderColor::White;
            minLod = MIN_LOD;
            maxLod = MAX_LOD;
            useMips = true;
        }
    };

    struct RHIUniformTextureDesc {
        int32 location;
        TPtrShared<class RHITexture> texture;
        TPtrShared<class RHISampler> sampler;
    };

    struct RHIUniformBlockDesc {
        int32 binding;
        uint32 range;
        uint32 offset;
        TPtrShared<class RHIUniformBuffer> buffer;
    };

    using RHIUniformTextures = TArrayStatic<RHIUniformTextureDesc, RHIConst::MAX_UNIFORM_TEXTURES>;
    using RHIUniformBlocks = TArrayStatic<RHIUniformBlockDesc, RHIConst::MAX_UNIFORM_BLOCKS>;

    struct RHIStencilStateDesc {
        bool enable : 1;
        uint8 writeMask;
        uint8 referenceValue;
        uint8 compareMask;
        EOperation sfail : 4;
        EOperation dfail : 4;
        EOperation dpass : 4;
        ECompareFunction compareFunction : 4;

        RHIStencilStateDesc() {
            enable = false;
            writeMask = 0;
            referenceValue = 0;
            compareMask = 0;
            sfail = EOperation::Keep;
            dfail = EOperation::Keep;
            dpass = EOperation::Keep;
            compareFunction = ECompareFunction::Never;
        }

        bool operator==(const RHIStencilStateDesc& desc) const {
            return (enable == desc.enable)
                && (writeMask == desc.writeMask)
                && (compareFunction == desc.compareFunction)
                && (referenceValue == desc.referenceValue)
                && (compareMask == desc.compareMask)
                && (sfail == desc.sfail)
                && (dfail == desc.dfail)
                && (dpass == desc.dpass);
        }
    };

    struct RHIBlendAttachment {
        bool enable : 1;
        EBlendOperation alphaBlendOp : 3;
        EBlendOperation colorBlendOp : 3;
        EBlendFactor srcAlphaBlendFactor : 4;
        EBlendFactor srcColorBlendFactor : 4;
        EBlendFactor dstAlphaBlendFactor : 4;
        EBlendFactor dstColorBlendFactor : 4;

        RHIBlendAttachment() {
            enable = false;
            alphaBlendOp = EBlendOperation::Add;
            colorBlendOp = EBlendOperation::Add;
            srcAlphaBlendFactor = EBlendFactor::One;
            srcColorBlendFactor = EBlendFactor::One;
            dstAlphaBlendFactor = EBlendFactor::One;
            dstColorBlendFactor = EBlendFactor::One;
        }

        bool operator==(const RHIBlendAttachment& other) const {
            return (!enable && !other.enable)
                || ( (enable == other.enable)
                 &&  (srcColorBlendFactor == other.srcColorBlendFactor)
                 &&  (dstColorBlendFactor == other.dstColorBlendFactor)
                 &&  (colorBlendOp == other.colorBlendOp)
                 &&  (srcAlphaBlendFactor == other.srcAlphaBlendFactor)
                 &&  (dstAlphaBlendFactor == other.dstAlphaBlendFactor)
                 &&  (alphaBlendOp == other.alphaBlendOp) );
        }
    };

    struct RHIBlendStateDesc {
        TArrayStatic<RHIBlendAttachment, RHIConst::MAX_COLOR_ATTACHMENTS> attachments;

        bool operator==(const RHIBlendStateDesc& desc) const {
            if (attachments.size() != desc.attachments.size())
                return false;

            for (uint32 i = 0; i < attachments.size(); i++) {
                if (!(attachments[i] == desc.attachments[i]))
                    return false;
            }

            return true;
        }
    };

}

#endif //BERSERK_RHIDESCS_H
