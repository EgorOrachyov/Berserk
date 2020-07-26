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
    };

    using RHIVertexDeclarationDesc = TArray<RHIVertexElement>;

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
        static const uint32 MIN_LOD = -100;
        static const uint32 MAX_LOD = 100;

        ESamplerFilter min = ESamplerFilter::Linear;
        ESamplerFilter mag = ESamplerFilter::Linear;
        ESamplerFilter mipmapMode = ESamplerFilter::Linear;
        ESamplerRepeatMode u = ESamplerRepeatMode::Repeat;
        ESamplerRepeatMode v = ESamplerRepeatMode::Repeat;
        ESamplerRepeatMode w = ESamplerRepeatMode::Repeat;
        ESamplerBorderColor color = ESamplerBorderColor::White;
        int32 minLod = MIN_LOD;
        int32 maxLod = MAX_LOD;
        bool useMips = true;
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

    struct RHIStencilStateDesc {
        bool enable = false;
        uint32 writeMask = 0;
        ECompareFunction compareFunction = ECompareFunction::Never;
        uint32 referenceValue = 0;
        uint32 compareMask = 0;
        EOperation sfail = EOperation::Keep;
        EOperation dfail = EOperation::Keep;
        EOperation dpass = EOperation::Keep;

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
        bool enable = false;
        EBlendOperation alphaBlendOp = EBlendOperation::Add;
        EBlendOperation colorBlendOp = EBlendOperation::Add;
        EBlendFactor srcAlphaBlendFactor = EBlendFactor::One;
        EBlendFactor srcColorBlendFactor = EBlendFactor::One;
        EBlendFactor dstAlphaBlendFactor = EBlendFactor::One;
        EBlendFactor dstColorBlendFactor = EBlendFactor::One;

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
        TArrayStatic<RHIBlendAttachment> attachments;

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

    struct RHIFramebufferFormatDesc {
        bool useDepthStencil = false;
        EPixelFormat depthStencilFormat = EPixelFormat::D24S8;
        TArrayStatic<EPixelFormat> colorFormats;

        bool operator==(const RHIFramebufferFormatDesc& other) const {
            bool equal = true;

            equal = equal && (useDepthStencil == other.useDepthStencil);
            equal = equal && (depthStencilFormat == other.depthStencilFormat);

            if (!equal || colorFormats.size() != other.colorFormats.size())
                return false;

            for (uint32 i = 0; i < colorFormats.size(); i++) {
                equal = equal && (colorFormats[i] == other.colorFormats[i]);
            }

            return equal;
        }
    };

    struct RHIGraphicsPipelineDesc {
        /** Hint, where we are going to render (useful for vulkan) */
        bool forWindowRendering = false;
        /** Compiled shader */
        TPtrShared<class RHIShader> shader;
        /** Layout of input vertices */
        TPtrShared<class RHIVertexDeclaration> vertexDeclaration;
        /** Raster properties */
        EPrimitivesType primitivesType = EPrimitivesType::Triangles;
        EPolygonMode polygonMode = EPolygonMode::Fill;
        EPolygonCullMode polygonCullMode = EPolygonCullMode::Back;
        EPolygonFrontFace polygonFrontFace = EPolygonFrontFace::CounterClockwise;
        float lineWidth = 1.0f;
        /** Depth setting */
        bool depthTest = true;
        bool depthWrite = true;
        ECompareFunction depthCompare = ECompareFunction::Less;
        /** Blend state per target color attachment */
        RHIBlendStateDesc blendState;
        /** Stencil test (default disable) */
        RHIStencilStateDesc stencilState;
        /** Format of the target framebuffer */
        RHIFramebufferFormatDesc framebufferFormat;
    };

}

#endif //BERSERK_RHIDESCS_H
