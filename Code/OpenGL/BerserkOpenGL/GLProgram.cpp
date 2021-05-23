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

#include <BerserkOpenGL/GLProgram.hpp>
#include <BerserkOpenGL/GLDefs.hpp>
#include <BerserkOpenGL/GLDriver.hpp>
#include <BerserkCore/Platform/ThreadManager.hpp>

namespace Berserk {
    namespace RHI {

        GLProgram::GLProgram(const Desc &desc) {
            mName = desc.name;
            mLanguage = desc.language;
            mStages = desc.stages;
        }

        GLProgram::~GLProgram() {
            if (mHandle) {
                GLuint shaders[Limits::MAX_SHADER_STAGES];
                GLsizei maxCount = Limits::MAX_SHADER_STAGES;
                GLsizei count;

                glGetAttachedShaders(mHandle, maxCount, &count, shaders);
                BERSERK_GL_CATCH_ERRORS();

                glDeleteProgram(mHandle);
                BERSERK_GL_CATCH_ERRORS();

                for (GLsizei i = 0; i < count; i++) {
                    glDeleteShader(shaders[i]);
                    BERSERK_GL_CATCH_ERRORS();
                }

                mHandle = 0;
                mCompilationStatus.store(false);
                mCompilerMessage = "";

                BERSERK_GL_LOG_INFO(BERSERK_TEXT("Release program: thread=\"{0}\""), ThreadManager::GetCurrentThread()->GetName());
            }
        }

        void GLProgram::Initialize() {
            assert(GLDriver::GetDevice().GetSupportedShaderLanguages().Contains(mLanguage));
            assert(ValidateStages());

            GLsizei current = 0;
            GLuint shaders[Limits::MAX_SHADER_STAGES];

            bool withError = false;
            String error;

            for (const auto& module: mStages) {
                assert(module.sourceCode);

                auto shaderType = GLDefs::GetShaderType(module.type);

                GLuint handle = glCreateShader(shaderType);
                BERSERK_GL_CATCH_ERRORS();

                const char* source = (const char*) module.sourceCode->GetData();
                const char* sources[] = { source };
                GLint length[] = { (GLint) module.sourceCode->GetSize() };

                glShaderSource(handle, 1, sources, length);
                BERSERK_GL_CATCH_ERRORS();

                glCompileShader(handle);
                BERSERK_GL_CATCH_ERRORS();

                GLint result = 0;
                glGetShaderiv(handle, GL_COMPILE_STATUS, &result);
                BERSERK_GL_CATCH_ERRORS();

                if (!result) {
                    withError = true;

                    int32 logLength;
                    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
                    BERSERK_GL_CATCH_ERRORS();

                    if (logLength > 0) {
                        String log(logLength + 1);

                        GLint written;
                        glGetShaderInfoLog(handle, logLength, &written, log.GetStr_C());
                        BERSERK_GL_CATCH_ERRORS();

                        assert(written <= logLength);
                        log.GetStr_C()[written] = String::END;

                        error = std::move(log);
                    }

                    glDeleteShader(handle);
                    BERSERK_GL_CATCH_ERRORS();

                    break;
                }

                shaders[current] = handle;
                current++;
            }

            if (withError) {
                // Release all created shader stages
                for (GLsizei i = 0; i < current; i++) {
                    glDeleteShader(shaders[i]);
                    BERSERK_GL_CATCH_ERRORS();
                }

                mCompilerMessage = std::move(error);
                mCompilationStatus.store((uint32) Status::FailedCompile);

                BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Shader \"{0}\" compilation: Error"), mName);
                return;
            }

            mHandle = glCreateProgram();
            BERSERK_GL_CATCH_ERRORS();

            for (GLsizei i = 0; i < current; i++) {
                glAttachShader(mHandle, shaders[i]);
                BERSERK_GL_CATCH_ERRORS();
            }

            glLinkProgram(mHandle);
            BERSERK_GL_CATCH_ERRORS();

            GLint status;
            glGetProgramiv(mHandle, GL_LINK_STATUS, &status);

            if (!status) {
                withError = true;

                int32 logLength;
                glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &logLength);
                BERSERK_GL_CATCH_ERRORS();

                if (logLength > 0) {
                    String log(logLength + 1);

                    GLint written;
                    glGetShaderInfoLog(mHandle, logLength, &written, log.GetStr_C());
                    BERSERK_GL_CATCH_ERRORS();

                    assert(written <= logLength);
                    log.GetStr_C()[written] = String::END;

                    error = std::move(log);
                }
            }

            if (withError) {
                // Release all created shader stages
                for (GLsizei i = 0; i < current; i++) {
                    glDeleteShader(shaders[i]);
                    BERSERK_GL_CATCH_ERRORS();
                }

                // Release program handle
                glDeleteProgram(mHandle);
                BERSERK_GL_CATCH_ERRORS();

                mHandle = 0;

                mCompilerMessage = std::move(error);
                mCompilationStatus.store((uint32) Status::FailedCompile);

                BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Shader \"{0}\" compilation: Error"), mName);
                return;
            }

            BERSERK_GL_LOG_INFO(BERSERK_TEXT("Shader \"{0}\" compilation: OK"), mName);

            // Create reflection meta information
            CreateProgramMeta();

            // Remember to notify user, that program is compiled
            mCompilationStatus.store((uint32) Status::Compiled, std::memory_order_release);
        }

        bool GLProgram::ValidateStages() const {
            // Currently only vs + fs shaders combination is allowed
            assert(mStages.GetSize() == 2);

            bool vs = false;
            bool fs = false;

            for (auto& stage: mStages) {
                if (stage.type == ShaderType::Vertex)
                    vs = true;
                else if (stage.type == ShaderType::Fragment)
                    fs = true;
                else
                    return false;
            }

            return vs && fs;
        }

        void GLProgram::CreateProgramMeta() {
            class GLProgramMeta final: public ProgramMeta {
            public:
                ~GLProgramMeta() override = default;

            protected:
                void OnReleased() const override {
                    Memory::Release(this);
                }
            };

            RefCounted<GLProgramMeta> meta(Memory::Make<GLProgramMeta>());

            auto& inputs = meta->inputs;
            auto& params = meta->params;
            auto& paramBlocks = meta->paramBlocks;
            auto& samplers = meta->samplers;

            // Input attributes

            GLint activeAttributes = 0;
            glGetProgramiv(mHandle, GL_ACTIVE_ATTRIBUTES, &activeAttributes);
            BERSERK_GL_CATCH_ERRORS();

            for (uint32 i = 0; i < activeAttributes; i++) {
                GLchar name[Limits::MAX_SHADER_PARAM_NAME];
                GLenum type;
                GLint size;
                GLsizei length;
                GLint location;

                glGetActiveAttrib(mHandle, i, Limits::MAX_SHADER_PARAM_NAME, &length, &size, &type, name);
                BERSERK_GL_CATCH_ERRORS();

                assert(length < Limits::MAX_SHADER_PARAM_NAME);

                location = glGetAttribLocation(mHandle, name);
                BERSERK_GL_CATCH_ERRORS();

                ProgramMeta::InputAttribute attribute;
                attribute.name = name;
                attribute.location = location;
                attribute.type = GLDefs::GetElementType(type);

                if (attribute.type != VertexElementType::Unknown && location != -1) {
                    inputs.Add(attribute.name, attribute);
                }
            }

            // Object Params

            GLint activeUniforms = 0;
            GLuint uniformsIndices[Limits::MAX_SHADER_PARAMS_COUNT];
            GLint uniformsOffset[Limits::MAX_SHADER_PARAMS_COUNT];
            GLint uniformsBlockIndices[Limits::MAX_SHADER_PARAMS_COUNT];
            GLint uniformsArrayStride[Limits::MAX_SHADER_PARAMS_COUNT];
            GLint uniformsMatrixStride[Limits::MAX_SHADER_PARAMS_COUNT];

            glGetProgramiv(mHandle, GL_ACTIVE_UNIFORMS, &activeUniforms);
            BERSERK_GL_CATCH_ERRORS();

            assert(activeUniforms <= Limits::MAX_SHADER_PARAMS_COUNT);

            for (uint32 i = 0; i < activeUniforms; i++) {
                uniformsIndices[i] = i;
            }

            glGetActiveUniformsiv(mHandle, activeUniforms, uniformsIndices, GL_UNIFORM_OFFSET, uniformsOffset);
            BERSERK_GL_CATCH_ERRORS();

            glGetActiveUniformsiv(mHandle, activeUniforms, uniformsIndices, GL_UNIFORM_BLOCK_INDEX, uniformsBlockIndices);
            BERSERK_GL_CATCH_ERRORS();

            glGetActiveUniformsiv(mHandle, activeUniforms, uniformsIndices, GL_UNIFORM_ARRAY_STRIDE, uniformsArrayStride);
            BERSERK_GL_CATCH_ERRORS();

            glGetActiveUniformsiv(mHandle, activeUniforms, uniformsIndices, GL_UNIFORM_MATRIX_STRIDE, uniformsMatrixStride);
            BERSERK_GL_CATCH_ERRORS();

            for (uint32 i = 0; i < activeUniforms; i++) {
                if (uniformsBlockIndices[i] >= 0) {
                    // This param within data param block block
                    continue;
                }

                GLchar name[Limits::MAX_SHADER_PARAM_NAME];
                GLsizei length;
                GLint location;
                GLint array;
                GLenum type;

                glGetActiveUniform(mHandle, i, Limits::MAX_SHADER_PARAM_NAME, &length, &array, &type, name);
                BERSERK_GL_CATCH_ERRORS();

                assert(length < Limits::MAX_SHADER_PARAM_NAME);

                location = glGetUniformLocation(mHandle, name);
                BERSERK_GL_CATCH_ERRORS();

                ProgramMeta::ObjectParam objectParam;
                objectParam.name = std::move(StringName(name, (StringUtils::FindLast(name, "]")? length - 3: length)));
                objectParam.location = location;
                objectParam.arraySize = array;
                objectParam.type = GLDefs::GetShaderParam(type);

                assert(objectParam.type != ShaderParamType::Unknown);
                samplers.Add(objectParam.name, objectParam);
            }

            // Data params within data param blocks (uniform blocks)

            GLint activeUniformBlocks = 0;
            glGetProgramiv(mHandle, GL_ACTIVE_UNIFORM_BLOCKS, &activeUniformBlocks);
            BERSERK_GL_CATCH_ERRORS();

            for (uint32 i = 0; i < activeUniformBlocks; i++) {
                GLchar name[Limits::MAX_SHADER_PARAM_NAME];
                GLsizei length;
                GLint size;
                GLint uniformsCount = 0;
                GLint membersIndices[Limits::MAX_SHADER_PARAMS_COUNT];

                glGetActiveUniformBlockName(mHandle, i, Limits::MAX_SHADER_PARAM_NAME, &length, name);
                BERSERK_GL_CATCH_ERRORS();

                glGetActiveUniformBlockiv(mHandle, i, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
                BERSERK_GL_CATCH_ERRORS();

                glGetActiveUniformBlockiv(mHandle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniformsCount);
                BERSERK_GL_CATCH_ERRORS();

                glGetActiveUniformBlockiv(mHandle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, membersIndices);
                BERSERK_GL_CATCH_ERRORS();

                assert(length < Limits::MAX_SHADER_PARAM_NAME);
                assert(uniformsCount < Limits::MAX_SHADER_PARAMS_COUNT);

                ProgramMeta::DataParamBlock paramBlock;
                paramBlock.name = name;
                paramBlock.slot = i;    // Actual index in the Shader program
                paramBlock.size = size;

                for (uint32 j = 0; j < uniformsCount; j++) {
                    GLenum type;
                    GLint array;
                    GLuint memberIndex = membersIndices[j];

                    glGetActiveUniform(mHandle, memberIndex, Limits::MAX_SHADER_PARAM_NAME, &length, &array, &type, name);
                    BERSERK_GL_CATCH_ERRORS();

                    assert(length < Limits::MAX_SHADER_PARAM_NAME);

                    ProgramMeta::DataParam dataParam;
                    dataParam.name = std::move(StringName(name, (StringUtils::FindLast(name, "]")? length - 3: length)));
                    dataParam.arraySize = array;
                    dataParam.arrayStride = uniformsArrayStride[memberIndex] > 0? uniformsArrayStride[memberIndex]: 0;
                    dataParam.elementSize = GLDefs::GetShaderDataSize(type);
                    dataParam.type = GLDefs::GetShaderDataParam(type);
                    dataParam.blockSlot = paramBlock.slot;
                    dataParam.blockOffset = uniformsOffset[memberIndex];

                    params.Add(dataParam.name, dataParam);
                }

                paramBlocks.Add(paramBlock.name, paramBlock);
            }

            mMeta = std::move((RefCounted<ProgramMeta>) meta);
        }

        void GLProgram::BindUniformBlock(uint32 binding) const {
            glUniformBlockBinding(mHandle, binding, binding);
            BERSERK_GL_CATCH_ERRORS();
        }

        void GLProgram::Use() const {
            glUseProgram(mHandle);
            BERSERK_GL_CATCH_ERRORS();
        }

        Program::Status GLProgram::GetCompilationStatus() const {
            return (Status) mCompilationStatus.load(std::memory_order_acquire);
        }

        String GLProgram::GetCompilerMessage() const {
            return GetCompilationStatus() != Status::PendingCompilation ? mCompilerMessage : String();
        }

        RefCounted<ProgramMeta> GLProgram::GetProgramMeta() const {
            return GetCompilationStatus() != Status::PendingCompilation ? mMeta : RefCounted<ProgramMeta>{};
        }

    }
}