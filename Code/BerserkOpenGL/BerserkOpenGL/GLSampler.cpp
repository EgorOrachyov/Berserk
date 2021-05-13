/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkOpenGL/GLSampler.hpp>
#include <BerserkOpenGL/GLDriver.hpp>
#include <BerserkOpenGL/GLDefs.hpp>

#include <BerserkCore/Platform/ThreadManager.hpp>

namespace Berserk {
    namespace RHI {

        GLSampler::GLSampler(const Desc &desc) {
            mState = desc;

            GLDriver::GetDeferredResourceContext().SubmitInit([this](){
                this->Initialize();
            });
        }

        GLSampler::~GLSampler() {
            if (mHandle) {
                glDeleteSamplers(1, &mHandle);
                BERSERK_GL_CATCH_ERRORS();

                mHandle = 0;
            }

            BERSERK_GL_LOG_INFO(BERSERK_TEXT("Release sampler: thread=\"{0}\""), ThreadManager::GetCurrentThread()->GetName());
        }

        void GLSampler::Initialize() {
            auto minFilter = GLDefs::GetSamplerMinFilter(mState.minFilter);
            auto magFilter = GLDefs::GetSamplerMagFilter(mState.magFilter);
            auto repeatS = GLDefs::GetSamplerRepeatMode(mState.u);
            auto repeatT = GLDefs::GetSamplerRepeatMode(mState.v);
            auto repeatR = GLDefs::GetSamplerRepeatMode(mState.w);
            auto color = GLDefs::GetBorderColor(mState.color);
            
            glGenSamplers(1, &mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glSamplerParameteri(mHandle, GL_TEXTURE_MIN_FILTER, minFilter);
            BERSERK_GL_CATCH_ERRORS();

            glSamplerParameteri(mHandle, GL_TEXTURE_MAG_FILTER, magFilter);
            BERSERK_GL_CATCH_ERRORS();

            glSamplerParameteri(mHandle, GL_TEXTURE_WRAP_S, repeatS);
            BERSERK_GL_CATCH_ERRORS();

            glSamplerParameteri(mHandle, GL_TEXTURE_WRAP_T, repeatT);
            BERSERK_GL_CATCH_ERRORS();

            glSamplerParameteri(mHandle, GL_TEXTURE_WRAP_R, repeatR);
            BERSERK_GL_CATCH_ERRORS();

            glSamplerParameterf(mHandle, GL_TEXTURE_MIN_LOD, mState.minLod);
            BERSERK_GL_CATCH_ERRORS();

            glSamplerParameterf(mHandle, GL_TEXTURE_MAX_LOD, mState.maxLod);
            BERSERK_GL_CATCH_ERRORS();

            glSamplerParameterfv(mHandle, GL_TEXTURE_BORDER_COLOR, color.GetValues());
            BERSERK_GL_CATCH_ERRORS();

            BERSERK_GL_LOG_INFO(BERSERK_TEXT("Init sampler: thread=\"{0}\""), ThreadManager::GetCurrentThread()->GetName());
        }

        void GLSampler::OnReleased() const {
            GLDriver::GetDeferredResourceContext().SubmitRelease([this](){
                Memory::Release(this);
            });
        }
    }
}