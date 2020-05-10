/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLSHADER_H
#define BERSERK_GLSHADER_H

#include <RHI/RHIResources.h>
#include <RHI/RHIDescs.h>
#include <GLDefinitions.h>
#include <GLErrors.h>

namespace Berserk {

    class GLShader : public RHIShader {
    public:

        ~GLShader() override {
            destroy();
        }

        bool create(const RHIShaderDesc& shaderDesc){
            RHIShaderViewDesc viewDesc;
            viewDesc.resize(shaderDesc.size());

            for (uint32 i = 0; i < shaderDesc.size(); i++) {
                viewDesc[i].type = shaderDesc[i].type;
                viewDesc[i].code = &shaderDesc[i].code;
            }

            return create(viewDesc);
        }

        bool create(const RHIShaderViewDesc& shaderDesc) {
            BERSERK_COND_ERROR_RET_VALUE(false, validate(shaderDesc), "Invalid shader descriptor");

            for (const auto& module: shaderDesc) {
                auto shaderType = GLDefinitions::getShaderType(module.type);

                GLuint handle = glCreateShader(shaderType);
                BERSERK_COND_ERROR_RET_VALUE(false, handle, "Failed to create shader");

                const char* source = (const char*) module.code->data();
                const char* sources[] = { source };

                glShaderSource(handle, 1, sources, nullptr);
                glCompileShader(handle);

                int32 result = 0;
                glGetShaderiv(handle, GL_COMPILE_STATUS, &result);

                if (!result) {
                    int32 logLength;
                    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);

                    if (logLength > 0) {
                        TArray<char> log;
                        log.resize(logLength + 1);

                        int32 written;
                        glGetShaderInfoLog(handle, logLength, &written, log.data());

                        BERSERK_ERROR("Compilation log: %s", log.data());
                    }

                    BERSERK_ERROR_RET_VALUE(false, "Failed to compile shader");
                }

                BERSERK_CATCH_OPENGL_ERRORS();

                mShaderStages.add(module.type);
                mShaderHandles.add(handle);
            }

            mProgramHandle = glCreateProgram();

            for (auto handle: mShaderHandles) {
                glAttachShader(mProgramHandle, handle);
            }

            glLinkProgram(mProgramHandle);

            int32 status;
            glGetProgramiv(mProgramHandle, GL_LINK_STATUS, &status);

            if (!status) {
                int32 logLength;
                glGetProgramiv(mProgramHandle, GL_INFO_LOG_LENGTH, &logLength);

                if (logLength > 0) {
                    TArray<char> log;
                    log.resize(logLength + 1);

                    int32 written;
                    glGetShaderInfoLog(mProgramHandle, logLength, &written, log.data());

                    BERSERK_ERROR("Compilation log: %s", log.data());
                }

                BERSERK_ERROR_RET_VALUE(false, "Failed to compile shader");
            }

            BERSERK_CATCH_OPENGL_ERRORS();

            return true;
        }

        void destroy() {
            if (mProgramHandle) {
                for (auto& handle: mShaderHandles) {
                    glDetachShader(mProgramHandle, handle);
                    glDeleteShader(handle);
                }

                glDeleteProgram(mProgramHandle);

                BERSERK_CATCH_OPENGL_ERRORS();

                mProgramHandle = 0;
                mShaderStages.clear();
                mShaderHandles.clear();    
            }
        }

        void bindUniformBlock(uint32 binding) const {
            glUniformBlockBinding(mProgramHandle, binding, binding);
        }

        GLuint getProgramHandle() const {
            return mProgramHandle;
        }

        static bool validate(const RHIShaderViewDesc& shaderDesc) {
            bool hasVertexShader = false;
            bool hasFragmentShader = false;

            for (const auto& module: shaderDesc) {
                if (module.type == EShaderType::Vertex) {
                    BERSERK_COND_ERROR_RET_VALUE(false, !hasVertexShader, "Already present");
                    hasVertexShader = true;
                    continue;
                }
                if (module.type == EShaderType::Fragment) {
                    BERSERK_COND_ERROR_RET_VALUE(false, !hasFragmentShader, "Already present");
                    hasFragmentShader = true;
                    continue;
                }
            }

            BERSERK_COND_ERROR_RET_VALUE(false, hasVertexShader && hasFragmentShader, "Incomplete shader pipeline");

            return true;
        }

    private:

        GLuint mProgramHandle = 0;
        TArrayStatic<GLuint> mShaderHandles;

    };

}

#endif //BERSERK_GLSHADER_H