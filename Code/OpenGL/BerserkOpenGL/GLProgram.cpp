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
            assert(mLanguage == ShaderLanguage::GLSL);

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
                mCompilationStatus.store((uint32) CompilationStatus::FailedCompile);

                BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Failed to compile shader: {0}"), mName);
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
                mCompilationStatus.store((uint32) CompilationStatus::FailedCompile);

                BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Failed to compile shader: {0}"), mName);
                return;
            }

            // Remember to notify user, that program is compiled
            mCompilationStatus.store((uint32) CompilationStatus::Compiled);

            BERSERK_GL_LOG_INFO(BERSERK_TEXT("Compiled Shader: {0}"), mName);
        }

        Program::CompilationStatus GLProgram::GetCompilationStatus() const {
            return (CompilationStatus) mCompilationStatus.load();
        }

        String GLProgram::GetCompilerMessage() const {
            return GetCompilationStatus() != CompilationStatus::PendingCompilation ? mCompilerMessage : String();
        }

    }
}