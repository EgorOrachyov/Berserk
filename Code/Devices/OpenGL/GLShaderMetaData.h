/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLSHADERMETADATA_H
#define BERSERK_GLSHADERMETADATA_H

#include <RHI/RHIResources.h>
#include <RHI/RHIDescs.h>
#include <GLDefinitions.h>
#include <GLErrors.h>
#include <GLShader.h>

namespace Berserk {

    class GLShaderMetaData : public RHIShaderMetaData {
    public:

        ~GLShaderMetaData() override = default;

        bool create(const TPtrShared<RHIShader> &shader) {
            BERSERK_COND_ERROR_RET_VALUE(false, shader.isNotNull(), "Shader must be valid");

            auto& GL_shader = (GLShader&)*shader;
            auto  GL_handle = GL_shader.getProgramHandle();

            GLint activeAttributes = 0;
            glGetProgramiv(GL_handle, GL_ACTIVE_ATTRIBUTES, &activeAttributes);

            mVertexShaderAttributes.ensureToAdd(activeAttributes);
            for (uint32 i = 0; i < activeAttributes; i++) {

                GLchar name[MAX_UNIFORM_NAME_LENGTH];
                GLenum type;
                GLint size;
                GLsizei length;
                GLint location;

                glGetActiveAttrib(GL_handle, i, MAX_UNIFORM_NAME_LENGTH, &length, &size, &type, name);
                BERSERK_COND_ERROR_RET_VALUE(false, length < MAX_UNIFORM_NAME_LENGTH, "Uniform attribute name too long");

                location = glGetAttribLocation(GL_handle, name);

                CString attributeName = name;
                uint32 attributeLocation = location;
                EShaderData attributeData = GLDefinitions::getShaderData(type);

                if (attributeData != EShaderData::Unknown && location != -1) {
                    mVertexShaderAttributes.emplace(attributeName, attributeLocation, attributeData);
                }
            }

            BERSERK_CATCH_OPENGL_ERRORS();

            GLint activeUniforms = 0;
            GLuint uniformsIndices[MAX_UNIFORMS_COUNT];
            GLint unifromsOffset[MAX_UNIFORMS_COUNT];
            GLint unifromsBlockIndices[MAX_UNIFORMS_COUNT];
            GLint unifromsArrayStride[MAX_UNIFORMS_COUNT];
            GLint unifromsMatrixStride[MAX_UNIFORMS_COUNT];
            GLint unifromsRowMajor[MAX_UNIFORMS_COUNT];

            glGetProgramiv(GL_handle, GL_ACTIVE_UNIFORMS, &activeUniforms);
            {
                BERSERK_COND_ERROR_RET_VALUE(false, activeUniforms < MAX_UNIFORMS_COUNT, "Active uniforms too much");

                for (uint32 i = 0; i < activeUniforms; i++) {
                    uniformsIndices[i] = i;
                }

                glGetActiveUniformsiv(GL_handle, activeUniforms, uniformsIndices, GL_UNIFORM_OFFSET, unifromsOffset);
                glGetActiveUniformsiv(GL_handle, activeUniforms, uniformsIndices, GL_UNIFORM_BLOCK_INDEX, unifromsBlockIndices);
                glGetActiveUniformsiv(GL_handle, activeUniforms, uniformsIndices, GL_UNIFORM_ARRAY_STRIDE, unifromsArrayStride);
                glGetActiveUniformsiv(GL_handle, activeUniforms, uniformsIndices, GL_UNIFORM_MATRIX_STRIDE, unifromsMatrixStride);
                glGetActiveUniformsiv(GL_handle, activeUniforms, uniformsIndices, GL_UNIFORM_IS_ROW_MAJOR, unifromsRowMajor);

                for (uint32 i = 0; i < activeUniforms; i++) {
                    if (unifromsBlockIndices[i] >= 0)
                        continue;

                    GLchar name[MAX_UNIFORM_NAME_LENGTH];
                    GLsizei length;
                    GLint location;
                    GLint array;
                    GLenum type;

                    glGetActiveUniform(GL_handle, i, MAX_UNIFORM_NAME_LENGTH, &length, &array, &type, name);
                    BERSERK_COND_ERROR_RET_VALUE(false, length < MAX_UNIFORM_NAME_LENGTH, "Uniform variable name too long");
                    location = glGetUniformLocation(GL_handle, name);

                    CString paramName(name, (array > 1? length - 3: length));
                    uint32 paramLocation = location;
                    EShaderData paramData = GLDefinitions::getShaderData(type);
                    TEnumMask<EShaderType> paramFlags = 0;

                    mParams.emplace(paramName, paramLocation, paramData, paramFlags);
                }
            }

            GLint activeUniformBlocks = 0;
            glGetProgramiv(GL_handle, GL_ACTIVE_UNIFORM_BLOCKS, &activeUniformBlocks);

            mUniformBlocks.ensureToAdd(activeUniformBlocks);
            for (uint32 i = 0; i < activeUniformBlocks; i++) {

                GLchar name[MAX_UNIFORM_NAME_LENGTH];
                GLsizei length;
                GLint binding;
                GLint size;
                GLint uniformsCount = 0;
                GLint membersIndices[MAX_UNIFORMS_COUNT];

                glGetActiveUniformBlockName(GL_handle, i, MAX_UNIFORM_NAME_LENGTH, &length, name);
                glGetActiveUniformBlockiv(GL_handle, i, GL_UNIFORM_BLOCK_BINDING, &binding);
                glGetActiveUniformBlockiv(GL_handle, i, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
                glGetActiveUniformBlockiv(GL_handle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniformsCount);
                glGetActiveUniformBlockiv(GL_handle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, membersIndices);

                BERSERK_COND_ERROR_RET_VALUE(false, length < MAX_UNIFORM_NAME_LENGTH, "Uniform block name too long");
                BERSERK_COND_ERROR_RET_VALUE(false, uniformsCount < MAX_UNIFORMS_COUNT, "Active uniforms in block too much");

                CString blockName = name;
                TArray<ShaderMember> blockMembers;
                uint32 blockBinding = binding;
                uint32 blockSize = size;
                TEnumMask<EShaderType> blockFlags = 0;

                for (uint32 j = 0; j < uniformsCount; j++) {
                    GLenum type;
                    GLint array;
                    GLuint memberIndex = membersIndices[j];

                    glGetActiveUniform(GL_handle, memberIndex, MAX_UNIFORM_NAME_LENGTH, &length, &array, &type, name);
                    BERSERK_COND_ERROR_RET_VALUE(false, length < MAX_UNIFORM_NAME_LENGTH, "Uniform name too long");

                    CString memberName(name, (array > 1? length - 3: length));
                    uint32 memberOffset = unifromsOffset[memberIndex];
                    uint32 memberStride = (unifromsArrayStride[memberIndex] > 0? unifromsArrayStride[memberIndex]: 0);
                    uint32 memberMatrixStride = (unifromsMatrixStride[memberIndex] > 0? unifromsMatrixStride[memberIndex]: 0);
                    uint32 memberElements = array;
                    uint32 memberSize = GLDefinitions::getShaderDataSize(type);
                    EShaderData memberBaseType = GLDefinitions::getShaderData(type);
                    bool memberRowMajor = (unifromsRowMajor[memberIndex] != 0);

                    blockMembers.emplace(memberName, memberOffset, memberStride, memberMatrixStride, memberElements, memberSize, memberBaseType, memberRowMajor);
                }

                mUniformBlocks.emplace(blockName, blockMembers, blockBinding, blockSize, blockFlags);
            }

            BERSERK_CATCH_OPENGL_ERRORS();

            return true;
        }

    };

}

#endif //BERSERK_GLSHADERMETADATA_H