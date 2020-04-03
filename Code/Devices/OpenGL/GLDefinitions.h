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
    };

}

#endif //BERSERK_GLDEFINITIONS_H