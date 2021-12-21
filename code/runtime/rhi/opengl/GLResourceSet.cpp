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

#include <rhi/opengl/GLBuffer.hpp>
#include <rhi/opengl/GLResourceSet.hpp>
#include <rhi/opengl/GLSampler.hpp>
#include <rhi/opengl/GLTexture.hpp>

#include <cassert>

BRK_NS_BEGIN

uint32 GLResourceBindingState::GetSlot(uint32 location) {
    auto &slot = mBoundSlots[location];
    if (slot.index == UNUSED_SLOT) slot.index = mNextSlot++;
    return slot.index;
}

void GLResourceBindingState::Clear() {
    mBoundSlots.clear();
    mNextSlot = 0;
}

GLResourceSet::GLResourceSet(const RHIResourceSetDesc &desc) {
    Update(desc);
}

void GLResourceSet::Update(const RHIResourceSetDesc &desc) {
    mTextures = desc.GetTextures();
    mSamplers = desc.GetSamplers();
    mBuffers = desc.GetBuffers();
}

void GLResourceSet::Bind(GLResourceBindingState &state, const Ref<GLShader> &shader) const {
    for (const auto &bind : mTextures) {
        assert(bind.texture.IsNotNull());
        assert(bind.texture->UsageShaderSampling());
        uint32 location = bind.location + bind.arrayIndex;
        uint32 slot = state.GetSlot(location);
        bind.texture.ForceCast<GLTexture>()->Bind(location, slot);
    }
    for (const auto &bind : mSamplers) {
        assert(bind.sampler.IsNotNull());
        uint32 location = bind.location + bind.arrayIndex;
        uint32 slot = state.GetSlot(location);
        bind.sampler.ForceCast<GLSampler>()->Bind(slot);
    }
    for (const auto &bind : mBuffers) {
        assert(bind.buffer.IsNotNull());
        bind.buffer.ForceCast<GLUniformBuffer>()->Bind(bind.location, bind.offset, bind.range);
        shader->BindUniformBlock(bind.location);
    }
}

BRK_NS_END