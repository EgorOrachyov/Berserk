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

#ifndef BERSERK_GLDEFS_HPP
#define BERSERK_GLDEFS_HPP

#include <GL/glew.h>

#include <core/io/Logger.hpp>
#include <core/math/TVecN.hpp>
#include <rhi/RHIDefs.hpp>

#ifdef BERSERK_DEBUG
    #define BRK_GL_CATCH_ERR() \
        do { GLDefs::CatchErrors(); } while (false);
#else
    #define BRK_GL_CATCH_ERR()
#endif

BRK_NS_BEGIN

/**
 * @defgroup opengl
 * @brief OpenGL 4.1+ based rhi implementation
 */

/**
 * @addtogroup opengl
 * @{
 */

/**
 * @class GLDefs
 * @brief GL mapping of RHI definitions
 */
class GLDefs {
public:
    static void CatchErrors() {
        GLenum error;

        while ((error = glGetError()) != GL_NO_ERROR) {
            BRK_ERROR("[GL] error desc: " << GetErrorDescription(error));
        }
    }

    static bool NeedClearBefore(RHIRenderTargetOption option) {
        switch (option) {
            case RHIRenderTargetOption::ClearStore:
            case RHIRenderTargetOption::ClearDiscard:
                return true;
            default:
                return false;
        }
    }

    static const char *GetErrorDescription(GLenum error) {
        switch (error) {
            case GL_INVALID_ENUM:
                return BRK_TEXT("Invalid enum passed");
            case GL_INVALID_VALUE:
                return BRK_TEXT("Invalid value passed");
            case GL_INVALID_OPERATION:
                return BRK_TEXT("Set of state for a command is not legal for the parameters given to command");
            case GL_OUT_OF_MEMORY:
                return BRK_TEXT("Out of memory");
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                return BRK_TEXT("Invalid framebuffer operation");
            default:
                return BRK_TEXT("Something else, refer to documentation");
        }
    }

    static GLenum GetBufferUsage(RHIBufferUsage bufferUsage) {
        switch (bufferUsage) {
            case RHIBufferUsage::Dynamic:
                return GL_DYNAMIC_DRAW;
            case RHIBufferUsage::Static:
                return GL_STATIC_DRAW;
            default:
                BRK_ERROR("Unsupported RHIBufferUsage");
                return GL_NONE;
        }
    }

    static GLenum GetShaderType(RHIShaderType type) {
        switch (type) {
            case RHIShaderType::Vertex:
                return GL_VERTEX_SHADER;
            case RHIShaderType::Fragment:
                return GL_FRAGMENT_SHADER;
            default:
                BRK_ERROR("Unsupported RHIShaderType");
                return GL_NONE;
        }
    }

    static GLenum GetIndexType(RHIIndexType type) {
        switch (type) {
            case RHIIndexType::Uint32:
                return GL_UNSIGNED_INT;
            case RHIIndexType::Uint16:
                return GL_UNSIGNED_SHORT;
            default:
                BRK_ERROR("Unsupported RHIIndexType");
                return GL_NONE;
        }
    }

    static GLenum GetIndexSize(RHIIndexType type) {
        switch (type) {
            case RHIIndexType::Uint32:
                return sizeof(uint32);
            case RHIIndexType::Uint16:
                return sizeof(uint16);
            default:
                BRK_ERROR("Unsupported RHIIndexType");
        }
    }

    static void GetVertexElementType(RHIVertexElementType type, GLenum &baseType, uint32 &count) {
        switch (type) {
            case RHIVertexElementType::Float1: {
                baseType = GL_FLOAT;
                count = 1;
                return;
            }
            case RHIVertexElementType::Float2: {
                baseType = GL_FLOAT;
                count = 2;
                return;
            }
            case RHIVertexElementType::Float3: {
                baseType = GL_FLOAT;
                count = 3;
                return;
            }
            case RHIVertexElementType::Float4: {
                baseType = GL_FLOAT;
                count = 4;
                return;
            }
            case RHIVertexElementType::Int1: {
                baseType = GL_INT;
                count = 1;
                return;
            }
            case RHIVertexElementType::Int2: {
                baseType = GL_INT;
                count = 2;
                return;
            }
            case RHIVertexElementType::Int3: {
                baseType = GL_INT;
                count = 3;
                return;
            }
            case RHIVertexElementType::Int4: {
                baseType = GL_INT;
                count = 4;
                return;
            }
            default:
                BRK_ERROR("Unsupported RHIVertexElementType");
        }
    }

    static GLenum GetTextureInternalFormat(RHITextureFormat format) {
        switch (format) {
            case RHITextureFormat::R8:
                return GL_R8;
            case RHITextureFormat::R8_SNORM:
                return GL_R8_SNORM;
            case RHITextureFormat::R16:
                return GL_R16;
            case RHITextureFormat::R16_SNORM:
                return GL_R16_SNORM;
            case RHITextureFormat::RG8:
                return GL_RG8;
            case RHITextureFormat::RG8_SNORM:
                return GL_RG8_SNORM;
            case RHITextureFormat::RG16:
                return GL_RG16;
            case RHITextureFormat::RG16_SNORM:
                return GL_RG16_SNORM;
            case RHITextureFormat::RGB8:
                return GL_RGB8;
            case RHITextureFormat::RGB8_SNORM:
                return GL_RGB8_SNORM;
            case RHITextureFormat::RGB16_SNORM:
                return GL_RGB16_SNORM;
            case RHITextureFormat::RGBA8:
                return GL_RGBA8;
            case RHITextureFormat::RGBA8_SNORM:
                return GL_RGBA8_SNORM;
            case RHITextureFormat::RGBA16:
                return GL_RGBA16;
            case RHITextureFormat::SRGB8:
                return GL_SRGB8;
            case RHITextureFormat::SRGB8_ALPHA8:
                return GL_SRGB8_ALPHA8;
            case RHITextureFormat::R16F:
                return GL_R16F;
            case RHITextureFormat::RG16F:
                return GL_RG16F;
            case RHITextureFormat::RGB16F:
                return GL_RGB16F;
            case RHITextureFormat::RGBA16F:
                return GL_RGBA16F;
            case RHITextureFormat::R32F:
                return GL_R32F;
            case RHITextureFormat::RG32F:
                return GL_RG32F;
            case RHITextureFormat::RGB32F:
                return GL_RGB32F;
            case RHITextureFormat::RGBA32F:
                return GL_RGBA32F;
            case RHITextureFormat::DEPTH32F:
                return GL_DEPTH_COMPONENT32F;
            case RHITextureFormat::DEPTH32F_STENCIL8:
                return GL_DEPTH32F_STENCIL8;
            case RHITextureFormat::DEPTH24_STENCIL8:
                return GL_DEPTH24_STENCIL8;
            default:
                BRK_ERROR("Unsupported RHITextureFormat");
                return GL_NONE;
        }
    }

    static bool IsSuitableForDepthStencil(RHITextureFormat format, bool &depth, bool &stencil) {
        switch (format) {
            case RHITextureFormat::DEPTH32F:
                depth = true;
                stencil = false;
                return true;
            case RHITextureFormat::DEPTH32F_STENCIL8:
            case RHITextureFormat::DEPTH24_STENCIL8:
                depth = true;
                stencil = true;
                return true;
            default:
                depth = false;
                stencil = false;
                return false;
        }
    }

    static GLenum GetTextureDataBaseFormat(RHITextureFormat format) {
        switch (format) {
            case RHITextureFormat::R8:
            case RHITextureFormat::R8_SNORM:
            case RHITextureFormat::R16:
            case RHITextureFormat::R16_SNORM:
            case RHITextureFormat::R16F:
            case RHITextureFormat::R32F:
                return GL_RED;
            case RHITextureFormat::RG8:
            case RHITextureFormat::RG8_SNORM:
            case RHITextureFormat::RG16:
            case RHITextureFormat::RG16_SNORM:
            case RHITextureFormat::RG16F:
            case RHITextureFormat::RG32F:
                return GL_RG;
            case RHITextureFormat::RGB8:
            case RHITextureFormat::RGB8_SNORM:
            case RHITextureFormat::RGB16_SNORM:
            case RHITextureFormat::RGB16F:
            case RHITextureFormat::RGB32F:
            case RHITextureFormat::SRGB8:
                return GL_RGB;
            case RHITextureFormat::RGBA8:
            case RHITextureFormat::RGBA8_SNORM:
            case RHITextureFormat::RGBA16:
            case RHITextureFormat::RGBA16F:
            case RHITextureFormat::RGBA32F:
            case RHITextureFormat::SRGB8_ALPHA8:
                return GL_RGBA;
            case RHITextureFormat::DEPTH32F:
                return GL_DEPTH_COMPONENT;
            case RHITextureFormat::DEPTH32F_STENCIL8:
            case RHITextureFormat::DEPTH24_STENCIL8:
                return GL_DEPTH_STENCIL;
            default:
                BRK_ERROR("Unsupported RHITextureFormat");
                return GL_NONE;
        }
    }

    static GLenum GetTextureDataType(RHITextureFormat format) {
        switch (format) {
            case RHITextureFormat::R8:
            case RHITextureFormat::RG8:
            case RHITextureFormat::RGB8:
            case RHITextureFormat::RGBA8:
            case RHITextureFormat::SRGB8:
            case RHITextureFormat::SRGB8_ALPHA8:
                return GL_UNSIGNED_BYTE;
            case RHITextureFormat::R8_SNORM:
            case RHITextureFormat::RG8_SNORM:
            case RHITextureFormat::RGB8_SNORM:
            case RHITextureFormat::RGBA8_SNORM:
                return GL_BYTE;
            case RHITextureFormat::R16:
            case RHITextureFormat::RG16:
            case RHITextureFormat::RGBA16:
                return GL_UNSIGNED_SHORT;
            case RHITextureFormat::R16_SNORM:
            case RHITextureFormat::RG16_SNORM:
            case RHITextureFormat::RGB16_SNORM:
                return GL_SHORT;
            case RHITextureFormat::R16F:
            case RHITextureFormat::RG16F:
            case RHITextureFormat::RGB16F:
            case RHITextureFormat::RGBA16F:
                return GL_HALF_FLOAT;
            case RHITextureFormat::R32F:
            case RHITextureFormat::RG32F:
            case RHITextureFormat::RGB32F:
            case RHITextureFormat::RGBA32F:
            case RHITextureFormat::DEPTH32F:
                return GL_FLOAT;
            case RHITextureFormat::DEPTH32F_STENCIL8:
                return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
            case RHITextureFormat::DEPTH24_STENCIL8:
                return GL_UNSIGNED_INT_24_8;
            default:
                BRK_ERROR("Unsupported RHITextureFormat");
                return GL_NONE;
        }
    }

    static GLenum GetTextureCubeFaceTarget(RHITextureCubemapFace face) {
        auto id = static_cast<uint32>(face);

        if (id >= RHILimits::MAX_TEXTURE_CUBE_FACES) {
            BRK_ERROR("Face id out of the range");
            return GL_NONE;
        }

        static const GLenum targets[] = {
                GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

        return targets[id];
    }

    static GLint GetSamplerMinFilter(RHISamplerMinFilter filter) {
        switch (filter) {
            case RHISamplerMinFilter::NearestMipmapNearest:
                return GL_NEAREST_MIPMAP_NEAREST;
            case RHISamplerMinFilter::LinearMipmapNearest:
                return GL_LINEAR_MIPMAP_NEAREST;
            case RHISamplerMinFilter::NearestMipmapLinear:
                return GL_NEAREST_MIPMAP_LINEAR;
            case RHISamplerMinFilter::LinearMipmapLinear:
                return GL_LINEAR_MIPMAP_LINEAR;
            case RHISamplerMinFilter::Nearest:
                return GL_NEAREST;
            case RHISamplerMinFilter::Linear:
                return GL_LINEAR;
            default:
                BRK_ERROR("Unsupported RHISamplerMinFilter");
                return GL_NONE;
        }
    }

    static GLint GetSamplerMagFilter(RHISamplerMagFilter filter) {
        switch (filter) {
            case RHISamplerMagFilter::Nearest:
                return GL_NEAREST;
            case RHISamplerMagFilter::Linear:
                return GL_LINEAR;
            default:
                BRK_ERROR("Unsupported RHISamplerMagFilter");
                return GL_NONE;
        }
    }

    static GLint GetSamplerRepeatMode(RHISamplerRepeatMode mode) {
        switch (mode) {
            case RHISamplerRepeatMode::Repeat:
                return GL_REPEAT;
            case RHISamplerRepeatMode::ClampToBorder:
                return GL_CLAMP_TO_BORDER;
            case RHISamplerRepeatMode::ClampToEdge:
                return GL_CLAMP_TO_EDGE;
            case RHISamplerRepeatMode::MirroredRepeat:
                return GL_MIRRORED_REPEAT;
            case RHISamplerRepeatMode::MirrorClamToEdge:
                return GL_MIRROR_CLAMP_TO_EDGE;
            default:
                BRK_ERROR("Unsupported RHISamplerRepeatMode");
                return GL_NONE;
        }
    }

    static Vec4f GetBorderColor(RHISamplerBorderColor color) {
        switch (color) {
            case RHISamplerBorderColor::Black:
                return {0.0f, 0.0f, 0.0f, 1.0f};
            case RHISamplerBorderColor::White:
                return {1.0f, 1.0f, 1.0f, 1.0f};
            default:
                BRK_ERROR("Unsupported RHISamplerBorderColor");
                return {};
        }
    }

    static GLenum GetPrimitivesType(RHIPrimitivesType type) {
        switch (type) {
            case RHIPrimitivesType::Triangles:
                return GL_TRIANGLES;
            case RHIPrimitivesType::Lines:
                return GL_LINES;
            case RHIPrimitivesType::Points:
                return GL_POINTS;
            default:
                BRK_ERROR("Unsupported RHIPrimitivesType");
                return GL_NONE;
        }
    }

    static GLenum GetPolygonMode(RHIPolygonMode mode) {
        switch (mode) {
            case RHIPolygonMode::Fill:
                return GL_FILL;
            case RHIPolygonMode::Line:
                return GL_LINE;
            case RHIPolygonMode::Point:
                return GL_POINT;
            default:
                BRK_ERROR("Unsupported RHIPolygonMode");
                return GL_NONE;
        }
    }

    static GLenum GetPolygonCullMode(RHIPolygonCullMode mode) {
        switch (mode) {
            case RHIPolygonCullMode::Front:
                return GL_FRONT;
            case RHIPolygonCullMode::Back:
                return GL_BACK;
            case RHIPolygonCullMode::Disabled:
                return GL_NONE;
            case RHIPolygonCullMode::FrontAndBack:
                return GL_FRONT_AND_BACK;
            default:
                BRK_ERROR("Unsupported RHIPolygonCullMode");
                return GL_NONE;
        }
    }

    static GLenum GetPolygonFrontFace(RHIPolygonFrontFace frontFace) {
        switch (frontFace) {
            case RHIPolygonFrontFace::Clockwise:
                return GL_CW;
            case RHIPolygonFrontFace::CounterClockwise:
                return GL_CCW;
            default:
                BRK_ERROR("Unsupported RHIPolygonFrontFace");
                return GL_NONE;
        }
    }

    static GLenum GetCompareFunc(RHICompareFunction function) {
        switch (function) {
            case RHICompareFunction::Never:
                return GL_NEVER;
            case RHICompareFunction::Less:
                return GL_LESS;
            case RHICompareFunction::Equal:
                return GL_EQUAL;
            case RHICompareFunction::LessEqual:
                return GL_LEQUAL;
            case RHICompareFunction::Greater:
                return GL_GREATER;
            case RHICompareFunction::GreaterEqual:
                return GL_GEQUAL;
            case RHICompareFunction::NotEqual:
                return GL_NOTEQUAL;
            case RHICompareFunction::Always:
                return GL_ALWAYS;
            default:
                BRK_ERROR("Unsupported RHICompareFunction");
                return GL_NONE;
        }
    }

    static GLenum GetBlendFactor(RHIBlendFactor factor) {
        switch (factor) {
            case RHIBlendFactor::Zero:
                return GL_ZERO;
            case RHIBlendFactor::One:
                return GL_ONE;
            case RHIBlendFactor::SrcColor:
                return GL_SRC_COLOR;
            case RHIBlendFactor::OneMinusSrcColor:
                return GL_ONE_MINUS_SRC_COLOR;
            case RHIBlendFactor::DstColor:
                return GL_DST_COLOR;
            case RHIBlendFactor::OneMinusDstColor:
                return GL_ONE_MINUS_DST_COLOR;
            case RHIBlendFactor::SrcAlpha:
                return GL_SRC_ALPHA;
            case RHIBlendFactor::OneMinusSrcAlpha:
                return GL_ONE_MINUS_SRC_ALPHA;
            case RHIBlendFactor::DstAlpha:
                return GL_DST_ALPHA;
            case RHIBlendFactor::OneMinusDstAlpha:
                return GL_ONE_MINUS_DST_ALPHA;
            default:
                BRK_ERROR("Unsupported RHIBlendFactor");
                return GL_NONE;
        }
    }

    static GLenum GetBlendOperation(RHIBlendOperation operation) {
        switch (operation) {
            case RHIBlendOperation::Add:
                return GL_FUNC_ADD;
            case RHIBlendOperation::Subtract:
                return GL_FUNC_SUBTRACT;
            case RHIBlendOperation::ReverseSubtract:
                return GL_FUNC_REVERSE_SUBTRACT;
            case RHIBlendOperation::Min:
                return GL_MIN;
            case RHIBlendOperation::Max:
                return GL_MAX;
            default:
                BRK_ERROR("Unsupported RHIBlendOperation");
                return GL_NONE;
        }
    }

    static RHIShaderDataType GetShaderDataParam(GLenum type) {
        switch (type) {
            case GL_FLOAT:
                return RHIShaderDataType::Float1;
            case GL_FLOAT_VEC2:
                return RHIShaderDataType::Float2;
            case GL_FLOAT_VEC3:
                return RHIShaderDataType::Float3;
            case GL_FLOAT_VEC4:
                return RHIShaderDataType::Float4;
            case GL_INT:
                return RHIShaderDataType::Int1;
            case GL_INT_VEC2:
                return RHIShaderDataType::Int2;
            case GL_INT_VEC3:
                return RHIShaderDataType::Int3;
            case GL_INT_VEC4:
                return RHIShaderDataType::Int4;
            case GL_UNSIGNED_INT:
                return RHIShaderDataType::Uint1;
            case GL_UNSIGNED_INT_VEC2:
                return RHIShaderDataType::Uint2;
            case GL_UNSIGNED_INT_VEC3:
                return RHIShaderDataType::Uint3;
            case GL_UNSIGNED_INT_VEC4:
                return RHIShaderDataType::Uint4;
            case GL_BOOL:
                return RHIShaderDataType::Bool1;
            case GL_BOOL_VEC2:
                return RHIShaderDataType::Bool2;
            case GL_BOOL_VEC3:
                return RHIShaderDataType::Bool3;
            case GL_BOOL_VEC4:
                return RHIShaderDataType::Bool4;
            case GL_FLOAT_MAT2:
                return RHIShaderDataType::Mat2;
            case GL_FLOAT_MAT3:
                return RHIShaderDataType::Mat3;
            case GL_FLOAT_MAT4:
                return RHIShaderDataType::Mat4;
            default:
                BRK_ERROR("Unsupported ShaderData");
                return RHIShaderDataType::Unknown;
        }
    }

    static bool IsMatrixType(GLenum type) {
        switch (type) {
            case GL_FLOAT_MAT2:
            case GL_FLOAT_MAT3:
            case GL_FLOAT_MAT4:
                return true;
            default:
                return false;
        }
    }

    static RHIShaderParamType GetShaderParam(GLenum type) {
        switch (type) {
            case GL_SAMPLER_2D:
                return RHIShaderParamType::Sampler2d;
            case GL_SAMPLER_2D_ARRAY:
                return RHIShaderParamType::Sampler2dArray;
            case GL_SAMPLER_3D:
                return RHIShaderParamType::Sampler3d;
            case GL_SAMPLER_CUBE:
                return RHIShaderParamType::SamplerCube;
            default:
                BRK_ERROR("Unsupported RHIShaderParamType");
                return RHIShaderParamType::Unknown;
        }
    }

    static RHIVertexElementType GetElementType(GLenum type) {
        switch (type) {
            case GL_FLOAT:
                return RHIVertexElementType::Float1;
            case GL_FLOAT_VEC2:
                return RHIVertexElementType::Float2;
            case GL_FLOAT_VEC3:
                return RHIVertexElementType::Float3;
            case GL_FLOAT_VEC4:
                return RHIVertexElementType::Float4;
            case GL_INT:
                return RHIVertexElementType::Int1;
            case GL_INT_VEC2:
                return RHIVertexElementType::Int2;
            case GL_INT_VEC3:
                return RHIVertexElementType::Int3;
            case GL_INT_VEC4:
                return RHIVertexElementType::Int4;
            default:
                BRK_ERROR("Unsupported RHIVertexElementType");
                return RHIVertexElementType::Unknown;
        }
    }

    static int32 GetShaderDataSize(GLenum type) {
        switch (type) {
            case GL_FLOAT:
                return sizeof(float) * 1;
            case GL_FLOAT_VEC2:
                return sizeof(float) * 2;
            case GL_FLOAT_VEC3:
                return sizeof(float) * 3;
            case GL_FLOAT_VEC4:
                return sizeof(float) * 4;
            case GL_INT:
                return sizeof(int32) * 1;
            case GL_INT_VEC2:
                return sizeof(int32) * 2;
            case GL_INT_VEC3:
                return sizeof(int32) * 3;
            case GL_INT_VEC4:
                return sizeof(int32) * 4;
            case GL_UNSIGNED_INT:
                return sizeof(uint32) * 1;
            case GL_UNSIGNED_INT_VEC2:
                return sizeof(uint32) * 2;
            case GL_UNSIGNED_INT_VEC3:
                return sizeof(uint32) * 3;
            case GL_UNSIGNED_INT_VEC4:
                return sizeof(uint32) * 4;
            case GL_BOOL:
                return sizeof(uint32) * 1;
            case GL_BOOL_VEC2:
                return sizeof(uint32) * 2;
            case GL_BOOL_VEC3:
                return sizeof(uint32) * 3;
            case GL_BOOL_VEC4:
                return sizeof(uint32) * 4;
            case GL_FLOAT_MAT2:
                return sizeof(float) * 2 * 2;
            case GL_FLOAT_MAT3:
                return sizeof(float) * 3 * 3;
            case GL_FLOAT_MAT4:
                return sizeof(float) * 4 * 4;
            case GL_SAMPLER_2D:
            case GL_SAMPLER_3D:
            case GL_SAMPLER_CUBE:
            default:
                return 0;
        }
    }

    static GLenum GetStencilOp(RHIOperation operation) {
        switch (operation) {
            case RHIOperation::Decrement:
                return GL_DECR;
            case RHIOperation::Increment:
                return GL_INCR;
            case RHIOperation::Invert:
                return GL_INVERT;
            case RHIOperation::Keep:
                return GL_KEEP;
            case RHIOperation::Replace:
                return GL_REPLACE;
            case RHIOperation::Zero:
                return GL_ZERO;
            default:
                BRK_ERROR("Unsupported RHIOperation");
                return GL_NONE;
        }
    }
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_GLDEFS_HPP
