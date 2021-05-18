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

#include <BerserkOpenGL/GLSampler.hpp>
#include <BerserkOpenGL/GLDefs.hpp>

#include <BerserkCore/Platform/ThreadManager.hpp>

namespace Berserk {
    namespace RHI {

        GLSampler::GLSampler(const Desc &desc) {
            mState = desc;
        }

        GLSampler::~GLSampler() {
            if (mHandle) {
                glDeleteSamplers(1, &mHandle);
                BERSERK_GL_CATCH_ERRORS();

                mHandle = 0;
                BERSERK_GL_LOG_INFO(BERSERK_TEXT("Release sampler: thread=\"{0}\""), ThreadManager::GetCurrentThread()->GetName());
            }
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

        void GLSampler::Bind(uint32 slot) const {
            glBindSampler(slot, mHandle);
            BERSERK_GL_CATCH_ERRORS();
        }
    }
}