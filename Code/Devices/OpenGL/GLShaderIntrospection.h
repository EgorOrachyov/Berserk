/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLSHADERINTROSPECTION_H
#define BERSERK_GLSHADERINTROSPECTION_H

#include <RHI/RHIResources.h>
#include <RHI/RHIDescs.h>
#include <GLDefinitions.h>
#include <GLErrors.h>
#include <GLShader.h>

namespace Berserk {

    class GLShaderIntrospection : public RHIShaderIntrospection {
    public:

        ~GLShaderIntrospection() override = default;

        void create(const TPtrShared<RHIShader> &shader) {
            BERSERK_COND_ERROR_FAIL(shader.isNotNull(), "Shader must be valid");

            auto& GL_shader = (GLShader&)*shader;
            auto  GL_handle = GL_shader.getProgramHandle();

            GLint activeAttributes = 0;
            glGetProgramiv(GL_handle, GL_ACTIVE_ATTRIBUTES, &activeAttributes);

            mVertexShaderAttributes.ensureToAdd(activeAttributes);
            for (uint32 i = 0; i < activeAttributes; i++) {
                GLenum type;
                GLint size;
                GLsizei length;
                const GLsizei bufferSize = MAX_UNIFORM_NAME_LENGTH;
                GLchar name[bufferSize];
                GLint location;
                EShaderData dataType;

                glGetActiveAttrib(GL_handle, i, bufferSize, &length, &size, &type, name);
                location = glGetAttribLocation(GL_handle, name);
                dataType = GLDefinitions::getShaderData(type);

                BERSERK_COND_ERROR_FAIL(length < MAX_UNIFORM_NAME_LENGTH, "Uniform attribute name too long");

                if (dataType != EShaderData::Unknown && location != -1) {
                    auto& attribute = mVertexShaderAttributes.emplace();
                    attribute.data = dataType;
                    attribute.name = name;
                    attribute.location = location;
                }
            }

            BERSERK_CATCH_OPENGL_ERRORS();

            GLint activeUniforms = 0;
            glGetProgramiv(GL_handle, GL_ACTIVE_UNIFORMS, &activeUniforms);
            {
                BERSERK_COND_ERROR_FAIL(activeUniforms < MAX_UNIFORMS_COUNT, "Active uniforms too much");

                GLuint uniformsIndices[MAX_UNIFORMS_COUNT];
                GLint unifromsOffset[MAX_UNIFORMS_COUNT];
                GLint unifromsBlockIndices[MAX_UNIFORMS_COUNT];
                GLint unifromsArrayStride[MAX_UNIFORMS_COUNT];
                GLint unifromsMatrixStride[MAX_UNIFORMS_COUNT];

                for (uint32 i = 0; i < activeUniforms; i++) {
                    uniformsIndices[i] = i;
                }

                glGetActiveUniformsiv(GL_handle, activeUniforms, uniformsIndices, GL_UNIFORM_OFFSET, unifromsOffset);
                glGetActiveUniformsiv(GL_handle, activeUniforms, uniformsIndices, GL_UNIFORM_BLOCK_INDEX, unifromsBlockIndices);
                glGetActiveUniformsiv(GL_handle, activeUniforms, uniformsIndices, GL_UNIFORM_ARRAY_STRIDE, unifromsArrayStride);
                glGetActiveUniformsiv(GL_handle, activeUniforms, uniformsIndices, GL_UNIFORM_MATRIX_STRIDE, unifromsMatrixStride);

                for (uint32 i = 0; i < activeUniforms; i++) {
                    GLsizei length;
                    const GLsizei bufferSize = MAX_UNIFORM_NAME_LENGTH;
                    GLchar name[bufferSize];
                    GLint location;
                    GLint stride;
                    GLint array;
                    GLenum type;
                    EShaderData dataType;

                    glGetActiveUniform(GL_handle, i, bufferSize, &length, &array, &type, name);
                    dataType = GLDefinitions::getShaderData(type);
                    location = glGetUniformLocation(GL_handle, name);
                    stride = (array > 1 ? unifromsArrayStride[i] : unifromsMatrixStride[i]);

                    if (array > 1)

                    BERSERK_COND_ERROR_FAIL(length < MAX_UNIFORM_NAME_LENGTH, "Uniform variable name too long");

                    if (dataType != EShaderData::Unknown) {
                        CString parameterName = name;
                        RHIShaderIntrospection::Parameter parameter{};
                        parameter.data = dataType;
                        parameter.location = (location == -1 ? DEFAULT_UNIFORM_LOCATION : location);
                        parameter.offset = (unifromsOffset[i] == -1 ? DEFAULT_UNIFORM_OFFSET : unifromsOffset[i]);
                        parameter.size = GLDefinitions::getShaderDataSize(type);
                        parameter.stageFlags = 0;
                        parameter.block = (unifromsBlockIndices[i] == -1? DEFAULT_UNIFORM_BLOCK_INDEX : unifromsBlockIndices[i]);
                        parameter.array = array;
                        parameter.stride = stride;

                        mParams.move(parameterName, parameter);
                    }
                }
            }

            GLint activeUniformBlocks = 0;
            glGetProgramiv(GL_handle, GL_ACTIVE_UNIFORM_BLOCKS, &activeUniformBlocks);

            mUniformBlocks.ensureToAdd(activeUniformBlocks);
            for (uint32 i = 0; i < activeUniformBlocks; i++) {
                GLsizei length;
                const GLsizei bufferSize = MAX_UNIFORM_NAME_LENGTH;
                GLchar name[bufferSize];
                GLint binding;
                GLint size;
                GLint uniformsCount = 0;
                GLint uniformsIndices[MAX_UNIFORMS_COUNT];

                glGetActiveUniformBlockName(GL_handle, i, bufferSize, &length, name);
                glGetActiveUniformBlockiv(GL_handle, i, GL_UNIFORM_BLOCK_BINDING, &binding);
                glGetActiveUniformBlockiv(GL_handle, i, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
                glGetActiveUniformBlockiv(GL_handle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniformsCount);
                glGetActiveUniformBlockiv(GL_handle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniformsIndices);

                BERSERK_COND_ERROR_FAIL(length < MAX_UNIFORM_NAME_LENGTH, "Uniform block name too long");
                BERSERK_COND_ERROR_FAIL(uniformsCount < MAX_UNIFORMS_COUNT, "Active uniforms in block too much");

                auto& buffer = mUniformBlocks.emplace();
                buffer.name = name;
                buffer.binding = binding;
                buffer.size = size;
                buffer.stageFlags = 0;
                buffer.params.ensureToAdd(uniformsCount);

                for (uint32 i = 0; i < uniformsCount; i++) {
                    glGetActiveUniformName(GL_handle, uniformsIndices[i], bufferSize, &length, name);

                    BERSERK_COND_ERROR_FAIL(length < MAX_UNIFORM_NAME_LENGTH, "Uniform name too long");

                    buffer.params.emplace(name);
                }
            }

            BERSERK_CATCH_OPENGL_ERRORS();

        }

    };

}

#endif //BERSERK_GLSHADERINTROSPECTION_H