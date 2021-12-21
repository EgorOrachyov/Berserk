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

#include <core/Engine.hpp>
#include <rhi/opengl/GLSampler.hpp>

BRK_NS_BEGIN

GLSampler::GLSampler(const RHISamplerDesc &desc) {
    mState = desc;
}

GLSampler::~GLSampler() {
    if (mHandle) {
        glDeleteSamplers(1, &mHandle);
        BRK_GL_CATCH_ERR();
        mHandle = 0;
    }
}

void GLSampler::Initialize() {
    const auto &caps = Engine::Instance().GetRHIDevice().GetCaps();
    auto anisotropy = MathUtils::Clamp(mState.maxAnisotropy, 1.0f, caps.maxAnisotropy);
    auto useAnisotropy = mState.useAnisotropy && caps.supportAnisotropy;

    auto minFilter = GLDefs::GetSamplerMinFilter(mState.minFilter);
    auto magFilter = GLDefs::GetSamplerMagFilter(mState.magFilter);
    auto repeatS = GLDefs::GetSamplerRepeatMode(mState.u);
    auto repeatT = GLDefs::GetSamplerRepeatMode(mState.v);
    auto repeatR = GLDefs::GetSamplerRepeatMode(mState.w);
    auto color = GLDefs::GetBorderColor(mState.color);

    glGenSamplers(1, &mHandle);
    BRK_GL_CATCH_ERR();

    glSamplerParameteri(mHandle, GL_TEXTURE_MIN_FILTER, minFilter);
    BRK_GL_CATCH_ERR();

    glSamplerParameteri(mHandle, GL_TEXTURE_MAG_FILTER, magFilter);
    BRK_GL_CATCH_ERR();

    glSamplerParameteri(mHandle, GL_TEXTURE_WRAP_S, repeatS);
    BRK_GL_CATCH_ERR();

    glSamplerParameteri(mHandle, GL_TEXTURE_WRAP_T, repeatT);
    BRK_GL_CATCH_ERR();

    glSamplerParameteri(mHandle, GL_TEXTURE_WRAP_R, repeatR);
    BRK_GL_CATCH_ERR();

    glSamplerParameterf(mHandle, GL_TEXTURE_MIN_LOD, mState.minLod);
    BRK_GL_CATCH_ERR();

    glSamplerParameterf(mHandle, GL_TEXTURE_MAX_LOD, mState.maxLod);
    BRK_GL_CATCH_ERR();

    glSamplerParameterfv(mHandle, GL_TEXTURE_BORDER_COLOR, color.GetData());
    BRK_GL_CATCH_ERR();

    if (useAnisotropy) {
        glSamplerParameterf(mHandle, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
        BRK_GL_CATCH_ERR();
    }
}

void GLSampler::Bind(uint32 slot) const {
    glBindSampler(slot, mHandle);
    BRK_GL_CATCH_ERR();
}

BRK_NS_END