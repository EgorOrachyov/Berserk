/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLDEFINITIONS_H
#define BERSERK_GLDEFINITIONS_H

#include <RHI/RHIDefinitions.h>
#include <ErrorMacro.h>
#include <glad/glad.h>

namespace Berserk {

    class GLDefinitions final {
    public:

        static GLenum getMemoryType(EMemoryType type) {
            switch (type) {
                case EMemoryType::Dynamic:
                    return GL_DYNAMIC_DRAW;
                case EMemoryType::Static:
                    return GL_STATIC_DRAW;
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static GLenum getShaderType(EShaderType type) {
            switch (type) {
                case EShaderType::Vertex:
                    return GL_VERTEX_SHADER;
                case EShaderType::Fragment:
                    return GL_FRAGMENT_SHADER;
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static GLenum getIndexType(EIndexType type) {
            switch (type) {
                case EIndexType::Uint32:
                    return GL_UNSIGNED_INT;
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static void getVertexElementType(EVertexElementType type, GLenum &baseType, uint32 &count) {
            switch (type) {
                case EVertexElementType::Float1: {
                    baseType = GL_FLOAT;
                    count = 1;
                    return;
                }
                case EVertexElementType::Float2: {
                    baseType = GL_FLOAT;
                    count = 2;
                    return;
                }
                case EVertexElementType::Float3: {
                    baseType = GL_FLOAT;
                    count = 3;
                    return;
                }
                case EVertexElementType::Float4: {
                    baseType = GL_FLOAT;
                    count = 4;
                    return;
                }
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static void getTexturePixelFormat(EPixelFormat pixelFormat, GLint &internalFormat, GLenum &format, GLenum &type) {
            switch (pixelFormat) {
                case EPixelFormat::R8: {
                    internalFormat = GL_R8;
                    format = GL_RED;
                    type = GL_UNSIGNED_BYTE;
                    return;
                }
                case EPixelFormat::R8G8B8A8: {
                    internalFormat = GL_RGBA8;
                    format = GL_RGBA;
                    type = GL_UNSIGNED_BYTE;
                    return;
                }
                case EPixelFormat::D24_S8: {
                    internalFormat = GL_DEPTH24_STENCIL8;
                    format = GL_DEPTH_STENCIL;
                    type = GL_UNSIGNED_INT_24_8;
                    return;
                }
                case EPixelFormat::D32_S8: {
                    internalFormat = GL_DEPTH32F_STENCIL8;
                    format = GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
                    type = GL_FLOAT;
                    return;
                }
                case EPixelFormat::R16G16B16A16f: {
                    internalFormat = GL_RGBA16F;
                    format = GL_RGBA;
                    type = GL_FLOAT;
                    return;
                }
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static GLenum getSamplerFiltering(ESamplerFilter filter, ESamplerFilter mips) {
            switch (mips) {
                case ESamplerFilter::Nearest: {
                    switch (filter) {
                        case ESamplerFilter::Nearest:
                            return GL_NEAREST_MIPMAP_NEAREST;
                        case ESamplerFilter::Linear:
                            return GL_LINEAR_MIPMAP_NEAREST;
                        default:
                            BERSERK_ERROR_FAIL("Unsupported format");
                    }
                }
                case ESamplerFilter::Linear: {
                    switch (filter) {
                        case ESamplerFilter::Nearest:
                            return GL_NEAREST_MIPMAP_LINEAR;
                        case ESamplerFilter::Linear:
                            return GL_LINEAR_MIPMAP_LINEAR;
                        default:
                            BERSERK_ERROR_FAIL("Unsupported format");
                    }
                }
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static GLenum getSamplerFiltering(ESamplerFilter filter) {
            switch (filter) {
                case ESamplerFilter::Nearest:
                    return GL_NEAREST;
                case ESamplerFilter::Linear:
                    return GL_LINEAR;
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static GLenum getSamplerRepeatMode(ESamplerRepeatMode mode) {
            switch (mode) {
                case ESamplerRepeatMode::Repeat:
                    return GL_REPEAT;
                case ESamplerRepeatMode::ClampToBorder:
                    return GL_CLAMP_TO_BORDER;
                case ESamplerRepeatMode::ClampToEdge:
                    return GL_CLAMP_TO_EDGE;
                case ESamplerRepeatMode::MirroredRepeat:
                    return GL_MIRRORED_REPEAT;
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static Color4f getBorderColor(ESamplerBorderColor color) {
            switch (color) {
                case ESamplerBorderColor::Black:
                    return Color4f(0.0f, 0.0f, 0.0f, 0.0f);
                case ESamplerBorderColor::White:
                    return Color4f(1.0f, 1.0f, 1.0f, 1.0f);
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static GLenum getPrimitivesType(EPrimitivesType type) {
            switch (type) {
                case EPrimitivesType::Triangles:
                    return GL_TRIANGLES;
                case EPrimitivesType::Lines:
                    return GL_LINES;
                case EPrimitivesType::Points:
                    return GL_POINTS;
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static GLenum getPolygonMode(EPolygonMode mode) {
            switch (mode) {
                case EPolygonMode::Fill:
                    return GL_FILL;
                case EPolygonMode::Line:
                    return GL_LINE;
                case EPolygonMode::Point:
                    return GL_POINT;
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static GLenum getPolygonCullMode(EPolygonCullMode mode) {
            switch (mode) {
                case EPolygonCullMode::Front:
                    return GL_FRONT;
                case EPolygonCullMode::Back:
                    return GL_BACK;
                case EPolygonCullMode::Disabled:
                    return GL_NONE;
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static GLenum getPolygonFrontFace(EPolygonFrontFace frontFace) {
            switch (frontFace) {
                case EPolygonFrontFace::Clockwise:
                    return GL_CW;
                case EPolygonFrontFace::CounterClockwise:
                    return GL_CCW;
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static GLenum getCompareFunc(ECompareFunction function) {
            switch (function) {
                case ECompareFunction::Never:
                    return GL_NEVER;
                case ECompareFunction::Less:
                    return GL_LESS;
                case ECompareFunction::Equal:
                    return GL_EQUAL;
                case ECompareFunction::LessEqual:
                    return GL_LEQUAL;
                case ECompareFunction::Greater:
                    return GL_GREATER;
                case ECompareFunction::GreaterEqual:
                    return GL_GEQUAL;
                case ECompareFunction::NotEqual:
                    return GL_NOTEQUAL;
                case ECompareFunction::Always:
                    return GL_ALWAYS;
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static GLenum getBlendFactor(EBlendFactor factor) {
            switch (factor) {
                case EBlendFactor::Zero:
                    return GL_ZERO;
                case EBlendFactor::One:
                    return GL_ONE;
                case EBlendFactor::SrcColor:
                    return GL_SRC_COLOR;
                case EBlendFactor::OneMinusSrcColor:
                    return GL_ONE_MINUS_SRC_COLOR;
                case EBlendFactor::DstColor:
                    return GL_DST_COLOR;
                case EBlendFactor::OneMinusDstColor:
                    return GL_ONE_MINUS_DST_COLOR;
                case EBlendFactor::SrcAlpha:
                    return GL_SRC_ALPHA;
                case EBlendFactor::OneMinusSrcAlpha:
                    return GL_ONE_MINUS_SRC_ALPHA;
                case EBlendFactor::DstAlpha:
                    return GL_DST_ALPHA;
                case EBlendFactor::OneMinusDstAlpha:
                    return GL_ONE_MINUS_DST_ALPHA;
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static GLenum getBlendOperation(EBlendOperation operation) {
            switch (operation) {
                case EBlendOperation::Add:
                    return GL_FUNC_ADD;
                case EBlendOperation::Subtract:
                    return GL_FUNC_SUBTRACT;
                case EBlendOperation::ReverseSubtract:
                    return GL_FUNC_REVERSE_SUBTRACT;
                case EBlendOperation::Min:
                    return GL_MIN;
                case EBlendOperation::Max:
                    return GL_MAX;
                default:
                    BERSERK_ERROR_FAIL("Unsupported format");
            }
        }

        static EShaderData getShaderData(GLenum type) {
            switch (type) {
                case GL_FLOAT:
                    return EShaderData::Float1;
                case GL_FLOAT_VEC2:
                    return EShaderData::Float2;
                case GL_FLOAT_VEC3:
                    return EShaderData::Float3;
                case GL_FLOAT_VEC4:
                    return EShaderData::Float4;
                case GL_FLOAT_MAT4:
                    return EShaderData::Mat4;
                case GL_SAMPLER_2D:
                    return EShaderData::Sampler2D;
                case GL_SAMPLER_3D:
                    return EShaderData::Sampler3D;
                case GL_SAMPLER_CUBE:
                    return EShaderData::SamplerCube;
                default:
                    return EShaderData::Unknown;
            }
        }

        static int32 getShaderDataSize(GLenum type) {
            switch (type) {
                case GL_FLOAT:
                    return sizeof(float32) * 1;
                case GL_FLOAT_VEC2:
                    return sizeof(float32) * 2;
                case GL_FLOAT_VEC3:
                    return sizeof(float32) * 3;
                case GL_FLOAT_VEC4:
                    return sizeof(float32) * 4;
                case GL_FLOAT_MAT2:
                    return sizeof(float32) * 2 * 2;
                case GL_FLOAT_MAT3:
                    return sizeof(float32) * 3 * 3;
                case GL_FLOAT_MAT4:
                    return sizeof(float32) * 4 * 4;
                case GL_SAMPLER_2D:
                case GL_SAMPLER_3D:
                case GL_SAMPLER_CUBE:
                default:
                    return 0;
            }
        }

    };

}

#endif //BERSERK_GLDEFINITIONS_H