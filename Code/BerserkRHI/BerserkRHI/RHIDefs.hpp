/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_RHIDEFS_HPP
#define BERSERK_RHIDEFS_HPP

#include <BerserkCore/Typedefs.hpp>

namespace Berserk {
    namespace RHI {

        enum class Type : uint8 {
            OpenGL,
            Vulkan,
            Unknown
        };

        /** Hint for device to allocate internally buffer memory in optimised way */
        enum class BufferUsage : uint8 {
            /** Update once, use many time */
            Static,
            /** Update many times, use many times */
            Dynamic
        };

        enum class IndexType : uint8 {
            Uint32,
            Uint16
        };

        /** Iterating of the elements in the vertex buffer */
        enum class VertexFrequency : uint8 {
            /** Element index changes per vertex */
            PerVertex = 0,
            /** Element index changes per instance */
            PerInstance
        };

        enum class VertexElementType : uint8 {
            Float1 = 0,
            Float2,
            Float3,
            Float4,
            Int1,
            Int2,
            Int3,
            Int4,
            Unknown
        };

        enum class ShaderDataType : uint8 {
            Float1,
            Float2,
            Float3,
            Float4,
            Int1,
            Int2,
            Int3,
            Int4,
            Uint1,
            Uint2,
            Uint3,
            Uint4,
            Bool1,
            Bool2,
            Bool3,
            Bool4,
            Mat2,
            Mat3,
            Mat4,
            Unknown
        };

        enum class ShaderParamType : uint8 {
            Sampler2d,
            Sampler2dArray,
            Sampler3d,
            SamplerCube,
            Unknown
        };

        enum class TextureType : uint8 {
            Texture2d,
            Texture2dArray,
            Texture3d,
            TextureCube,
            Unknown
        };

        enum class ShaderType : uint8 {
            Vertex = 0,
            Fragment = 1
        };

        enum class ShaderLanguage : uint8 {
            GLSL410GL = 1,
            GLSL450GL = 2,
            GLSL450VK = 3,
            Unknown
        };

        /** Formats used to specify internal texture storage format */
        enum class TextureFormat : uint8 {
            R8,
            R8_SNORM,
            R16,
            R16_SNORM,
            RG8,
            RG8_SNORM,
            RG16,
            RG16_SNORM,
            RGB8,
            RGB8_SNORM,
            RGB16_SNORM,
            RGBA8,
            RGBA8_SNORM,
            RGBA16,
            SRGB8,
            SRGB8_ALPHA8,
            R16F,
            RG16F,
            RGB16F,
            RGBA16F,
            R32F,
            RG32F,
            RGB32F,
            RGBA32F,
            DEPTH32F,
            DEPTH32F_STENCIL8,
            DEPTH24_STENCIL8,
            Unknown
        };

        enum class TextureUsage : uint8 {
            /** Data frequently uploaded to this texture (directly from Cpu) */
            CanUpdate = 1,
            /** Texture can be sampled within shader */
            Sampling = 2,
            /** Texture can be used as render target color attachment */
            ColorAttachment = 3,
            /** Texture can be used as render target depth attachment */
            DepthAttachment = 4,
            /** Texture can be used as render target depth stencil attachment */
            DepthStencilAttachment = 5,
        };

        enum class TextureCubemapFace : uint8 {
            PositiveX = 0,
            NegativeX = 1,
            PositiveY = 2,
            NegativeY = 3,
            PositiveZ = 4,
            NegativeZ = 5,
        };

        enum class SamplerMagFilter : uint8 {
            Nearest,
            Linear
        };

        enum class SamplerMinFilter : uint8 {
            /** Returns the value of the texture element that is nearest */
            Nearest,
            /** Returns the weighted average of the four texture elements that are closest to the center of the pixel being textured */
            Linear,
            /** Chooses the mipmap that most closely matches the size of the pixel being textured and uses the Nearest criterion */
            NearestMipmapNearest,
            /** Chooses the mipmap that most closely matches the size of the pixel being textured and uses the Linear criterion */
            LinearMipmapNearest,
            /** Chooses the two mipmaps that most closely match the size of the pixel being textured and uses the Nearest criterion */
            NearestMipmapLinear,
            /** Chooses the two mipmaps that most closely match the size of the pixel being textured and uses the Linear criterion */
            LinearMipmapLinear
        };

        enum class SamplerRepeatMode : uint8 {
            Repeat,
            MirroredRepeat,
            ClampToEdge,
            ClampToBorder,
            MirrorClamToEdge,
        };

        enum class SamplerBorderColor : uint8 {
            Black,
            White
        };

        /** Types of the geometry formed by input vertex data */
        enum class PrimitivesType : uint8 {
            Triangles,
            Lines,
            Points
        };

        /** How primitives rasterized */
        enum class PolygonMode : uint8 {
            Fill,
            Line,
            Point
        };

        enum class PolygonCullMode : uint8 {
            Disabled,
            Front,
            Back,
            FrontAndBack
        };

        /** How front of the primitive is defined */
        enum class PolygonFrontFace : uint8 {
            Clockwise,
            CounterClockwise
        };

        enum class CompareFunction : uint8 {
            Never,
            Less,
            Equal,
            LessEqual,
            Greater,
            GreaterEqual,
            NotEqual,
            Always
        };

        enum class Operation : uint8 {
            Keep,
            Zero,
            Replace,
            Increment,
            Decrement,
            Invert
        };

        enum class BlendFactor : uint8 {
            /** = 0 */
            Zero,
            /** = 1 */
            One,
            /** = src */
            SrcColor,
            /** = 1 - src */
            OneMinusSrcColor,
            /** = dst */
            DstColor,
            /** = 1 - dst */
            OneMinusDstColor,
            /** = src.a */
            SrcAlpha,
            /** = 1 - src.a */
            OneMinusSrcAlpha,
            /** = dst.a */
            DstAlpha,
            /** = 1 - dst.a  */
            OneMinusDstAlpha
        };

        enum class BlendOperation : uint8 {
            /** Fragment color added to the color buffer */
            Add,
            /** Fragment color subtracted from the color buffer */
            Subtract,
            /** Color buffer color is subtracted from fragment color  */
            ReverseSubtract,
            /** The min between the fragment and color buffer */
            Min,
            /** The max between the fragment and color buffer */
            Max
        };

        enum class RenderTargetOption : uint8 {
            ClearStore,
            ClearDiscard,
            LoadStore,
            LoadDiscard,
            DiscardStore,
            DiscardDiscard
        };

        struct Limits {
            /** Vertex shader max input elements */
            static const uint32 MAX_VERTEX_ATTRIBUTES = 8;

            /** Vertex shader max input vertex buffers (same as elements) */
            static const uint32 MAX_VERTEX_BUFFERS = 8;

            /**  Limit somehow color attachment count */
            static const uint32 MAX_COLOR_ATTACHMENTS = 8;

            /** Max shader param name */
            static const uint32 MAX_SHADER_PARAM_NAME = 1024;

            /** Max shader params count */
            static const uint32 MAX_SHADER_PARAMS_COUNT = 1024;

            /** Max number of mipmaps for a single texture object */
            static const uint32 MAX_TEXTURE_MIPMAPS = 16;

            /** Max number of the shader stages within single shader program */
            static const uint32 MAX_SHADER_STAGES = 2;

            /** Max number of cube texture faces (it is cube, lol) */
            static const uint32 MAX_TEXTURE_CUBE_FACES = 6;

            /** Max number of simultaneously drawn frames (1 - frames drawn and presented in the sequence, 2 - two frames overlap, ...)*/
            static const uint32 MAX_FRAMES_IN_FLIGHT = 3;
        };

        struct DeviceCaps {
            uint32 maxVertexAttributes;
            uint32 maxCombinedUniformBlocks;
            uint32 maxTextureArrayLayers;
            uint32 maxTexture3dSize;
            uint32 maxTextureSize;
            uint32 maxTextureUnits;
            uint32 maxColorAttachments;
            uint32 maxFramebufferWidth;
            uint32 maxFramebufferHeight;
            uint32 uniformBlockOffsetAlignment;
        };

    }
}

#endif //BERSERK_RHIDEFS_HPP
