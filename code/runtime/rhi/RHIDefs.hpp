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

#include <core/Config.hpp>
#include <core/Typedefs.hpp>

#include <core/string/String.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup rhi
 * @{
 */

enum class RHIType : uint8 {
    OpenGL,
    Vulkan,
    Unknown
};

/** Hint for device to allocate internally buffer memory in optimised way */
enum class RHIBufferUsage : uint8 {
    /** Update once, use many time */
    Static,
    /** Update many times, use many times */
    Dynamic
};

enum class RHIIndexType : uint8 {
    Uint32,
    Uint16
};

/** Iterating of the elements in the vertex buffer */
enum class RHIVertexFrequency : uint8 {
    /** Element index changes per vertex */
    PerVertex = 0,
    /** Element index changes per instance */
    PerInstance
};

enum class RHIVertexElementType : uint8 {
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

enum class RHIShaderDataType : uint8 {
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

enum class RHIShaderParamType : uint8 {
    Sampler2d,
    Sampler2dArray,
    Sampler3d,
    SamplerCube,
    Unknown
};

enum class RHITextureType : uint8 {
    Texture2d,
    Texture2dArray,
    Texture3d,
    TextureCube,
    Unknown
};

enum class RHIShaderType : uint8 {
    Vertex = 0,
    Fragment = 1
};

enum class RHIShaderLanguage : uint8 {
    GLSL410GL = 1,
    GLSL450GL = 2,
    GLSL450VK = 3,
    Unknown
};

/** Formats used to specify internal texture storage format */
enum class RHITextureFormat : uint8 {
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

enum class RHITextureUsage : uint8 {
    /** Texture can be sampled within shader */
    Sampling = 1,
    /** Texture can be used as render target color attachment */
    ColorAttachment = 2,
    /** Texture can be used as render target depth attachment */
    DepthAttachment = 3,
    /** Texture can be used as render target depth stencil attachment */
    DepthStencilAttachment = 4,
};

enum class RHITextureCubemapFace : uint8 {
    PositiveX = 0,
    NegativeX = 1,
    PositiveY = 2,
    NegativeY = 3,
    PositiveZ = 4,
    NegativeZ = 5,
};

enum class RHISamplerMagFilter : uint8 {
    Nearest,
    Linear
};

enum class RHISamplerMinFilter : uint8 {
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

enum class RHISamplerRepeatMode : uint8 {
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder,
    MirrorClamToEdge,
};

enum class RHISamplerBorderColor : uint8 {
    Black,
    White
};

/** Types of the geometry formed by input vertex data */
enum class RHIPrimitivesType : uint8 {
    Triangles,
    Lines,
    Points
};

/** How primitives rasterized */
enum class RHIPolygonMode : uint8 {
    Fill,
    Line,
    Point
};

enum class RHIPolygonCullMode : uint8 {
    Disabled,
    Front,
    Back,
    FrontAndBack
};

/** How front of the primitive is defined */
enum class RHIPolygonFrontFace : uint8 {
    Clockwise,
    CounterClockwise
};

enum class RHICompareFunction : uint8 {
    Never,
    Less,
    Equal,
    LessEqual,
    Greater,
    GreaterEqual,
    NotEqual,
    Always
};

enum class RHIOperation : uint8 {
    Keep,
    Zero,
    Replace,
    Increment,
    Decrement,
    Invert
};

enum class RHIBlendFactor : uint8 {
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

enum class RHIBlendOperation : uint8 {
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

enum class RHIRenderTargetOption : uint8 {
    ClearStore,
    ClearDiscard,
    LoadStore,
    LoadDiscard,
    DiscardStore,
    DiscardDiscard
};

/** @brief RHI common device limits */
struct RHILimits {
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

    /** Max number of simultaneously drawn frames (1 - draw frame and wait, 2 - draw and present one frame, prepare other, ...)*/
    static const uint32 MAX_FRAMES_IN_FLIGHT = 2;

    /** Max resources sets count */
    static const uint32 MAX_RESOURCE_SETS = 3;
};

/** @brief RHI device capabilities */
struct RHIDeviceCaps {
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
    float maxAnisotropy;
    bool supportAnisotropy;
};

/** @return Host data size of value of specified type */
inline uint32 GetRHIShaderDataTypeSize(RHIShaderDataType type) {
    const uint32 fBase = 4;
    const uint32 iBase = 4;
    const uint32 uBase = 4;
    const uint32 bBase = 4;

    switch (type) {
        case RHIShaderDataType::Float1:
            return fBase * 1;
        case RHIShaderDataType::Float2:
            return fBase * 2;
        case RHIShaderDataType::Float3:
            return fBase * 3;
        case RHIShaderDataType::Float4:
            return fBase * 4;
        case RHIShaderDataType::Int1:
            return iBase * 1;
        case RHIShaderDataType::Int2:
            return iBase * 2;
        case RHIShaderDataType::Int3:
            return iBase * 3;
        case RHIShaderDataType::Int4:
            return iBase * 4;
        case RHIShaderDataType::Uint1:
            return uBase * 1;
        case RHIShaderDataType::Uint2:
            return uBase * 2;
        case RHIShaderDataType::Uint3:
            return uBase * 3;
        case RHIShaderDataType::Uint4:
            return uBase * 4;
        case RHIShaderDataType::Bool1:
            return bBase * 1;
        case RHIShaderDataType::Bool2:
            return bBase * 2;
        case RHIShaderDataType::Bool3:
            return bBase * 3;
        case RHIShaderDataType::Bool4:
            return bBase * 4;
        case RHIShaderDataType::Mat2:
            return fBase * 2 * 2;
        case RHIShaderDataType::Mat3:
            return fBase * 3 * 3;
        case RHIShaderDataType::Mat4:
            return fBase * 4 * 4;
        default:
            return 0;
    }
}

inline const char *RHIGetShaderDataIdGLSL(RHIShaderDataType type) {
    switch (type) {
        case RHIShaderDataType::Float1:
            return "float";
        case RHIShaderDataType::Float2:
            return "vec2";
        case RHIShaderDataType::Float3:
            return "vec3";
        case RHIShaderDataType::Float4:
            return "vec4";
        case RHIShaderDataType::Int1:
            return "int";
        case RHIShaderDataType::Int2:
            return "ivec2";
        case RHIShaderDataType::Int3:
            return "ivec3";
        case RHIShaderDataType::Int4:
            return "ivec4";
        case RHIShaderDataType::Uint1:
            return "uint";
        case RHIShaderDataType::Uint2:
            return "uvec2";
        case RHIShaderDataType::Uint3:
            return "uvec3";
        case RHIShaderDataType::Uint4:
            return "uvec4";
        case RHIShaderDataType::Bool1:
            return "bool";
        case RHIShaderDataType::Bool2:
            return "bvec2";
        case RHIShaderDataType::Bool3:
            return "bvec3";
        case RHIShaderDataType::Bool4:
            return "bvec4";
        case RHIShaderDataType::Mat2:
            return "mat2";
        case RHIShaderDataType::Mat3:
            return "mat3";
        case RHIShaderDataType::Mat4:
            return "mat4";
        default:
            return "__none__";
    }
}

inline const char* RHIGetShaderParamIdGLSL(RHIShaderParamType type) {
    switch (type) {
        case RHIShaderParamType::Sampler2d:
            return "sampler2D";
        case RHIShaderParamType::Sampler2dArray:
            return "sampler2DArray";
        case RHIShaderParamType::Sampler3d:
            return "sampler3D";
        case RHIShaderParamType::SamplerCube:
            return "samplerCube";
        default:
            return "__none__";
    }
}

inline bool RHIIsIntegralBaseType(RHIShaderDataType type) {
    switch (type) {
        case RHIShaderDataType::Int1:
        case RHIShaderDataType::Int2:
        case RHIShaderDataType::Int3:
        case RHIShaderDataType::Int4:
        case RHIShaderDataType::Uint1:
        case RHIShaderDataType::Uint2:
        case RHIShaderDataType::Uint3:
        case RHIShaderDataType::Uint4:
        case RHIShaderDataType::Bool1:
        case RHIShaderDataType::Bool2:
        case RHIShaderDataType::Bool3:
        case RHIShaderDataType::Bool4:
            return true;
        default:
            return false;
    }
}

inline bool RHIIsFloatBaseType(RHIShaderDataType type) {
    switch (type) {
        case RHIShaderDataType::Float1:
        case RHIShaderDataType::Float2:
        case RHIShaderDataType::Float3:
        case RHIShaderDataType::Float4:
        case RHIShaderDataType::Mat2:
        case RHIShaderDataType::Mat3:
        case RHIShaderDataType::Mat4:
            return true;
        default:
            return false;
    }
}

inline bool RHIIsIntegralBaseSignedType(RHIShaderDataType type) {
    switch (type) {
        case RHIShaderDataType::Int1:
        case RHIShaderDataType::Int2:
        case RHIShaderDataType::Int3:
        case RHIShaderDataType::Int4:
            return true;
        default:
            return false;
    }
}

inline bool RHIIsVectorType(RHIShaderDataType type) {
    switch (type) {
        case RHIShaderDataType::Float1:
        case RHIShaderDataType::Float2:
        case RHIShaderDataType::Float3:
        case RHIShaderDataType::Float4:
        case RHIShaderDataType::Int1:
        case RHIShaderDataType::Int2:
        case RHIShaderDataType::Int3:
        case RHIShaderDataType::Int4:
        case RHIShaderDataType::Uint1:
        case RHIShaderDataType::Uint2:
        case RHIShaderDataType::Uint3:
        case RHIShaderDataType::Uint4:
        case RHIShaderDataType::Bool1:
        case RHIShaderDataType::Bool2:
        case RHIShaderDataType::Bool3:
        case RHIShaderDataType::Bool4:
            return true;
        default:
            return false;
    }
}

inline uint32 RHIGetVectorComponentsCount(RHIShaderDataType type) {
    switch (type) {
        case RHIShaderDataType::Float1:
        case RHIShaderDataType::Int1:
        case RHIShaderDataType::Uint1:
        case RHIShaderDataType::Bool1:
            return 1;
        case RHIShaderDataType::Float2:
        case RHIShaderDataType::Int2:
        case RHIShaderDataType::Uint2:
        case RHIShaderDataType::Bool2:
            return 2;
        case RHIShaderDataType::Float3:
        case RHIShaderDataType::Int3:
        case RHIShaderDataType::Uint3:
        case RHIShaderDataType::Bool3:
            return 3;
        case RHIShaderDataType::Float4:
        case RHIShaderDataType::Int4:
        case RHIShaderDataType::Uint4:
        case RHIShaderDataType::Bool4:
            return 4;
        default:
            return 0;
    }
}

inline void RHIGetMatrixSize(RHIShaderDataType type, uint32 &rows, uint32 &cols) {
    switch (type) {
        case RHIShaderDataType::Mat2:
            rows = cols = 2u;
            return;
        case RHIShaderDataType::Mat3:
            rows = cols = 3u;
            return;
        case RHIShaderDataType::Mat4:
            rows = cols = 4u;
            return;
        default:
            return;
    }
}

inline RHIShaderLanguage RHIParseShaderLanguage(const char *language) {
    static String g_GLSL410GL("GLSL410GL");
    static String g_GLSL450GL("GLSL450GL");
    static String g_GLSL450VK("GLSL450VK");

    if (g_GLSL410GL == language)
        return RHIShaderLanguage::GLSL410GL;
    else if (g_GLSL450GL == language)
        return RHIShaderLanguage::GLSL450GL;
    else if (g_GLSL450VK == language)
        return RHIShaderLanguage::GLSL450VK;
    else
        return RHIShaderLanguage::Unknown;
}

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RHIDEFS_HPP
