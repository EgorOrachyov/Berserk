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
#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Image/Color.hpp>

#define BERSERK_LOG_GL BERSERK_TEXT("GL")

#define BERSERK_GL_LOG_INFO(...) \
        Debug::GetDebugLog().LogMessageF(BERSERK_LOG_GL, Log::Verbosity::Info, __VA_ARGS__);

#define BERSERK_GL_LOG_ERROR(...) \
        Debug::GetDebugLog().LogMessageF(BERSERK_LOG_GL, Log::Verbosity::Error, __VA_ARGS__);

#ifdef BERSERK_DEBUG
    #define BERSERK_GL_CATCH_ERRORS() \
            do { GLDefs::CatchErrors(); } while(false);
#else
    #define BERSERK_GL_CATCH_ERRORS()
#endif

namespace Berserk {
    namespace RHI {

        class GLDefs {
        public:

            static void CatchErrors() {
                GLenum error;

                while ((error = glGetError()) != GL_NO_ERROR) {
                    BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Description: {0}"), GetErrorDescription(error));
                }
            }

            static const char* GetErrorDescription(GLenum error) {
                switch (error) {
                    case GL_INVALID_ENUM:
                        return BERSERK_TEXT("Invalid enum passed");
                    case GL_INVALID_VALUE:
                        return BERSERK_TEXT("Invalid value passed");
                    case GL_INVALID_OPERATION:
                        return BERSERK_TEXT("Set of state for a command is not legal for the parameters given to command");
                    case GL_OUT_OF_MEMORY:
                        return BERSERK_TEXT("Out of memory");
                    case GL_INVALID_FRAMEBUFFER_OPERATION:
                        return BERSERK_TEXT("Invalid framebuffer operation");
                    default:
                        return BERSERK_TEXT("Something else, refer to documentation");
                }
            }

            static GLenum GetBufferUsage(BufferUsage bufferUsage) {
                switch (bufferUsage) {
                    case BufferUsage::Dynamic:
                        return GL_DYNAMIC_DRAW;
                    case BufferUsage::Static:
                        return GL_STATIC_DRAW;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported BufferUsage"));
                        return GL_NONE;
                }
            }

            static GLenum GetShaderType(ShaderType type) {
                switch (type) {
                    case ShaderType::Vertex:
                        return GL_VERTEX_SHADER;
                    case ShaderType::Fragment:
                        return GL_FRAGMENT_SHADER;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported ShaderType"));
                        return GL_NONE;
                }
            }

            static GLenum GetIndexType(IndexType type) {
                switch (type) {
                    case IndexType::Uint32:
                        return GL_UNSIGNED_INT;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported IndexType"));
                }
            }

            static GLenum GetIndexSize(IndexType type) {
                switch (type) {
                    case IndexType::Uint32:
                        return sizeof(uint32);
                    case IndexType::Uint16:
                        return sizeof(uint16);
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported IndexType"));
                }
            }

            static void GetVertexElementType(VertexElementType type, GLenum &baseType, uint32 &count) {
                switch (type) {
                    case VertexElementType::Float1: {
                        baseType = GL_FLOAT;
                        count = 1;
                        return;
                    }
                    case VertexElementType::Float2: {
                        baseType = GL_FLOAT;
                        count = 2;
                        return;
                    }
                    case VertexElementType::Float3: {
                        baseType = GL_FLOAT;
                        count = 3;
                        return;
                    }
                    case VertexElementType::Float4: {
                        baseType = GL_FLOAT;
                        count = 4;
                        return;
                    }
                    case VertexElementType::Int1: {
                        baseType = GL_INT;
                        count = 1;
                        return;
                    }
                    case VertexElementType::Int2: {
                        baseType = GL_INT;
                        count = 2;
                        return;
                    }
                    case VertexElementType::Int3: {
                        baseType = GL_INT;
                        count = 3;
                        return;
                    }
                    case VertexElementType::Int4: {
                        baseType = GL_INT;
                        count = 4;
                        return;
                    }
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported VertexElementType"));
                }
            }

            static GLenum GetPixelDataFormat(PixelDataFormat format) {
                switch (format) {
                    case PixelDataFormat::R:
                        return GL_RED;
                    case PixelDataFormat::RG:
                        return GL_RG;
                    case PixelDataFormat::RGB:
                        return GL_RGB;
                    case PixelDataFormat::RGBA:
                        return GL_RGBA;
                    case PixelDataFormat::DEPTH_COMPONENT:
                        return GL_DEPTH_COMPONENT;
                    case PixelDataFormat::DEPTH_STENCIL:
                        return GL_DEPTH_STENCIL;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported PixelDataFormat"));
                        return GL_NONE;
                }
            }

            static GLenum GetPixelDataType(PixelDataType type) {
                switch (type) {
                    case PixelDataType::UBYTE:
                        return GL_UNSIGNED_BYTE;
                    case PixelDataType::BYTE:
                        return GL_BYTE;
                    case PixelDataType::USHORT:
                        return GL_UNSIGNED_SHORT;
                    case PixelDataType::SHORT:
                        return GL_SHORT;
                    case PixelDataType::UINT:
                        return GL_UNSIGNED_INT;
                    case PixelDataType::INT:
                        return GL_INT;
                    case PixelDataType::HALF:
                        return GL_HALF_FLOAT;
                    case PixelDataType::FLOAT:
                        return GL_FLOAT;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported PixelDataType"));
                        return GL_NONE;
                }
            }

            static GLenum GetTextureInternalFormat(TextureFormat format) {
                switch (format) {
                    case TextureFormat::R8:
                        return GL_R8;
                    case TextureFormat::R8_SNORM:
                        return GL_R8_SNORM;
                    case TextureFormat::R16:
                        return GL_R16;
                    case TextureFormat::R16_SNORM:
                        return GL_R16_SNORM;
                    case TextureFormat::RG8:
                        return GL_RG8;
                    case TextureFormat::RG8_SNORM:
                        return GL_RG8_SNORM;
                    case TextureFormat::RG16:
                        return GL_RG16;
                    case TextureFormat::RG16_SNORM:
                        return GL_RG16_SNORM;
                    case TextureFormat::RGB8:
                        return GL_RGB8;
                    case TextureFormat::RGB8_SNORM:
                        return GL_RGB8_SNORM;
                    case TextureFormat::RGB16_SNORM:
                        return GL_RGB16_SNORM;
                    case TextureFormat::RGBA8:
                        return GL_RGBA8;
                    case TextureFormat::RGBA8_SNORM:
                        return GL_RGBA8_SNORM;
                    case TextureFormat::RGBA16:
                        return GL_RGBA16;
                    case TextureFormat::SRGB8:
                        return GL_SRGB8;
                    case TextureFormat::SRGB8_ALPHA8:
                        return GL_SRGB8_ALPHA8;
                    case TextureFormat::R16F:
                        return GL_R16F;
                    case TextureFormat::RG16F:
                        return GL_RG16F;
                    case TextureFormat::RGB16F:
                        return GL_RGB16F;
                    case TextureFormat::RGBA16F:
                        return GL_RGBA16F;
                    case TextureFormat::R32F:
                        return GL_R32F;
                    case TextureFormat::RG32F:
                        return GL_RG32F;
                    case TextureFormat::RGB32F:
                        return GL_RGB32F;
                    case TextureFormat::RGBA32F:
                        return GL_RGBA32F;
                    case TextureFormat::DEPTH32F:
                        return GL_DEPTH_COMPONENT32F;
                    case TextureFormat::DEPTH32F_STENCIL8:
                        return GL_DEPTH32F_STENCIL8;
                    case TextureFormat::DEPTH24_STENCIL8:
                        return GL_DEPTH24_STENCIL8;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported TextureFormat"));
                        return GL_NONE;
                }
            }

//            static void GetTexturePixelFormat(TextureFormat textureFormat, GLint &internalFormat, GLenum &format, GLenum &type) {
//                switch (textureFormat) {
//                    case TextureFormat::R8: {
//                        internalFormat = GL_R8;
//                        format = GL_RED;
//                        type = GL_UNSIGNED_BYTE;
//                        return;
//                    }
//                    case TextureFormat::R8G8B8A8: {
//                        internalFormat = GL_RGBA8;
//                        format = GL_RGBA;
//                        type = GL_UNSIGNED_BYTE;
//                        return;
//                    }
////                    case TextureFormat::D24S8: {
////                        internalFormat = GL_DEPTH24_STENCIL8;
////                        format = GL_DEPTH_STENCIL;
////                        type = GL_UNSIGNED_INT_24_8;
////                        return;
////                    }
////                    case TextureFormat::D32S8: {
////                        internalFormat = GL_DEPTH32F_STENCIL8;
////                        format = GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
////                        type = GL_FLOAT;
////                        return;
////                    }
////                    case TextureFormat::R16G16B16A16f: {
////                        internalFormat = GL_RGBA16F;
////                        format = GL_RGBA;
////                        type = GL_FLOAT;
////                        return;
////                    }
//                    default:
//                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported PixelFormat"));
//                }
//            }

            static GLenum GetSamplerMinFilter(SamplerMinFilter filter) {
                switch (filter) {
                    case SamplerMinFilter::NearestMipmapNearest:
                        return GL_NEAREST_MIPMAP_NEAREST;
                    case SamplerMinFilter::LinearMipmapNearest:
                        return GL_LINEAR_MIPMAP_NEAREST;
                    case SamplerMinFilter::NearestMipmapLinear:
                        return GL_NEAREST_MIPMAP_LINEAR;
                    case SamplerMinFilter::LinearMipmapLinear:
                        return GL_LINEAR_MIPMAP_LINEAR;
                    case SamplerMinFilter::Nearest:
                        return GL_NEAREST;
                    case SamplerMinFilter::Linear:
                        return GL_LINEAR;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported SamplerMinFilter"));
                        return GL_NONE;
                }
            }

            static GLenum GetSamplerMagFilter(SamplerMagFilter filter) {
                switch (filter) {
                    case SamplerMagFilter::Nearest:
                        return GL_NEAREST;
                    case SamplerMagFilter::Linear:
                        return GL_LINEAR;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported SamplerMagFilter"));
                        return GL_NONE;
                }
            }

            static GLenum GetSamplerRepeatMode(SamplerRepeatMode mode) {
                switch (mode) {
                    case SamplerRepeatMode::Repeat:
                        return GL_REPEAT;
                    case SamplerRepeatMode::ClampToBorder:
                        return GL_CLAMP_TO_BORDER;
                    case SamplerRepeatMode::ClampToEdge:
                        return GL_CLAMP_TO_EDGE;
                    case SamplerRepeatMode::MirroredRepeat:
                        return GL_MIRRORED_REPEAT;
                    case SamplerRepeatMode::MirrorClamToEdge:
                        return GL_MIRROR_CLAMP_TO_EDGE;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported SamplerRepeatMode"));
                        return GL_NONE;
                }
            }

            static Color GetBorderColor(SamplerBorderColor color) {
                switch (color) {
                    case SamplerBorderColor::Black:
                        return {0.0f, 0.0f, 0.0f, 1.0f};
                    case SamplerBorderColor::White:
                        return {1.0f, 1.0f, 1.0f, 1.0f};
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT( "Unsupported SamplerBorderColor"));
                        return {};
                }
            }

            static GLenum GetPrimitivesType(PrimitivesType type) {
                switch (type) {
                    case PrimitivesType::Triangles:
                        return GL_TRIANGLES;
                    case PrimitivesType::Lines:
                        return GL_LINES;
                    case PrimitivesType::Points:
                        return GL_POINTS;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported PrimitivesType"));
                        return GL_NONE;
                }
            }

            static GLenum GetPolygonMode(PolygonMode mode) {
                switch (mode) {
                    case PolygonMode::Fill:
                        return GL_FILL;
                    case PolygonMode::Line:
                        return GL_LINE;
                    case PolygonMode::Point:
                        return GL_POINT;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported PolygonMode"));
                        return GL_NONE;
                }
            }

            static GLenum GetPolygonCullMode(PolygonCullMode mode) {
                switch (mode) {
                    case PolygonCullMode::Front:
                        return GL_FRONT;
                    case PolygonCullMode::Back:
                        return GL_BACK;
                    case PolygonCullMode::Disabled:
                        return GL_NONE;
                    case PolygonCullMode::FrontAndBack:
                        return GL_FRONT_AND_BACK;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported PolygonCullMode"));
                        return GL_NONE;
                }
            }

            static GLenum GetPolygonFrontFace(PolygonFrontFace frontFace) {
                switch (frontFace) {
                    case PolygonFrontFace::Clockwise:
                        return GL_CW;
                    case PolygonFrontFace::CounterClockwise:
                        return GL_CCW;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported PolygonFrontFace"));
                }
            }

            static GLenum GetCompareFunc(CompareFunction function) {
                switch (function) {
                    case CompareFunction::Never:
                        return GL_NEVER;
                    case CompareFunction::Less:
                        return GL_LESS;
                    case CompareFunction::Equal:
                        return GL_EQUAL;
                    case CompareFunction::LessEqual:
                        return GL_LEQUAL;
                    case CompareFunction::Greater:
                        return GL_GREATER;
                    case CompareFunction::GreaterEqual:
                        return GL_GEQUAL;
                    case CompareFunction::NotEqual:
                        return GL_NOTEQUAL;
                    case CompareFunction::Always:
                        return GL_ALWAYS;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported CompareFunction"));
                        return GL_NONE;
                }
            }

            static GLenum GetBlendFactor(BlendFactor factor) {
                switch (factor) {
                    case BlendFactor::Zero:
                        return GL_ZERO;
                    case BlendFactor::One:
                        return GL_ONE;
                    case BlendFactor::SrcColor:
                        return GL_SRC_COLOR;
                    case BlendFactor::OneMinusSrcColor:
                        return GL_ONE_MINUS_SRC_COLOR;
                    case BlendFactor::DstColor:
                        return GL_DST_COLOR;
                    case BlendFactor::OneMinusDstColor:
                        return GL_ONE_MINUS_DST_COLOR;
                    case BlendFactor::SrcAlpha:
                        return GL_SRC_ALPHA;
                    case BlendFactor::OneMinusSrcAlpha:
                        return GL_ONE_MINUS_SRC_ALPHA;
                    case BlendFactor::DstAlpha:
                        return GL_DST_ALPHA;
                    case BlendFactor::OneMinusDstAlpha:
                        return GL_ONE_MINUS_DST_ALPHA;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported BlendFactor"));
                        return GL_NONE;
                }
            }

            static GLenum GetBlendOperation(BlendOperation operation) {
                switch (operation) {
                    case BlendOperation::Add:
                        return GL_FUNC_ADD;
                    case BlendOperation::Subtract:
                        return GL_FUNC_SUBTRACT;
                    case BlendOperation::ReverseSubtract:
                        return GL_FUNC_REVERSE_SUBTRACT;
                    case BlendOperation::Min:
                        return GL_MIN;
                    case BlendOperation::Max:
                        return GL_MAX;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported BlendOperation"));
                        return GL_NONE;
                }
            }

            static ShaderDataType GetShaderDataParam(GLenum type) {
                switch (type) {
                    case GL_FLOAT:
                        return ShaderDataType::Float1;
                    case GL_FLOAT_VEC2:
                        return ShaderDataType::Float2;
                    case GL_FLOAT_VEC3:
                        return ShaderDataType::Float3;
                    case GL_FLOAT_VEC4:
                        return ShaderDataType::Float4;
                    case GL_INT:
                        return ShaderDataType::Int1;
                    case GL_INT_VEC2:
                        return ShaderDataType::Int2;
                    case GL_INT_VEC3:
                        return ShaderDataType::Int3;
                    case GL_INT_VEC4:
                        return ShaderDataType::Int4;
                    case GL_UNSIGNED_INT:
                        return ShaderDataType::Uint1;
                    case GL_UNSIGNED_INT_VEC2:
                        return ShaderDataType::Uint2;
                    case GL_UNSIGNED_INT_VEC3:
                        return ShaderDataType::Uint3;
                    case GL_UNSIGNED_INT_VEC4:
                        return ShaderDataType::Uint4;
                    case GL_BOOL:
                        return ShaderDataType::Bool1;
                    case GL_BOOL_VEC2:
                        return ShaderDataType::Bool2;
                    case GL_BOOL_VEC3:
                        return ShaderDataType::Bool3;
                    case GL_BOOL_VEC4:
                        return ShaderDataType::Bool4;
                    case GL_FLOAT_MAT2:
                        return ShaderDataType::Mat2;
                    case GL_FLOAT_MAT3:
                        return ShaderDataType::Mat3;
                    case GL_FLOAT_MAT4:
                        return ShaderDataType::Mat4;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported ShaderData"));
                        return ShaderDataType::Unknown;
                }
            }

            static ShaderParamType GetShaderParam(GLenum type) {
                switch (type) {
                    case GL_SAMPLER_2D:
                        return ShaderParamType::Sampler2d;
                    case GL_SAMPLER_2D_ARRAY:
                        return ShaderParamType::Sampler2dArray;
                    case GL_SAMPLER_3D:
                        return ShaderParamType::Sampler3d;
                    case GL_SAMPLER_CUBE:
                        return ShaderParamType::SamplerCube;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported ShaderParamType"));
                        return ShaderParamType::Unknown;
                }
            }

            static VertexElementType GetElementType(GLenum type) {
                switch (type) {
                    case GL_FLOAT:
                        return VertexElementType::Float1;
                    case GL_FLOAT_VEC2:
                        return VertexElementType::Float2;
                    case GL_FLOAT_VEC3:
                        return VertexElementType::Float3;
                    case GL_FLOAT_VEC4:
                        return VertexElementType::Float4;
                    case GL_INT:
                        return VertexElementType::Int1;
                    case GL_INT_VEC2:
                        return VertexElementType::Int2;
                    case GL_INT_VEC3:
                        return VertexElementType::Int3;
                    case GL_INT_VEC4:
                        return VertexElementType::Int4;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported VertexElementType"));
                        return VertexElementType::Unknown;
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

            static GLenum GetStencilOp(Operation operation) {
                switch (operation) {
                    case Operation::Decrement:
                        return GL_DECR;
                    case Operation::Increment:
                        return GL_INCR;
                    case Operation::Invert:
                        return GL_INVERT;
                    case Operation::Keep:
                        return GL_KEEP;
                    case Operation::Replace:
                        return GL_REPLACE;
                    case Operation::Zero:
                        return GL_ZERO;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported Operation"));
                }
            }

        };

    }
}

#endif //BERSERK_GLDEFS_HPP
