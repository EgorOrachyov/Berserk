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

#include <rhi/RHIResourceSet.hpp>

#include <algorithm>

BRK_NS_BEGIN

void RHIResourceSetDesc::AddTexture(Ref<RHITexture> texture, uint32 location) {
    TextureBinding binding;
    binding.location = location;
    binding.texture = std::move(texture);
    mTextures.push_back(std::move(binding));
}

void RHIResourceSetDesc::AddSampler(Ref<RHISampler> sampler, uint32 location) {
    SamplerBinding binding;
    binding.location = location;
    binding.sampler = std::move(sampler);
    mSamplers.push_back(std::move(binding));
}

void RHIResourceSetDesc::AddBuffer(Ref<RHIUniformBuffer> buffer, uint32 location) {
    BufferBinding binding;
    binding.location = location;
    binding.buffer = std::move(buffer);
    mBuffers.push_back(std::move(binding));
}

bool RHIResourceSetDesc::SetTexture(Ref<RHITexture> texture, uint32 location) {
    auto query = std::find_if(mTextures.begin(), mTextures.end(), [=](const TextureBinding &b) { return b.location == location; });
    if (query != mTextures.end()) {
        query->texture = std::move(texture);
        return true;
    }
    AddTexture(std::move(texture), location);
    return false;
}

bool RHIResourceSetDesc::SetSampler(Ref<RHISampler> sampler, uint32 location) {
    auto query = std::find_if(mSamplers.begin(), mSamplers.end(), [=](const SamplerBinding &b) { return b.location == location; });
    if (query != mSamplers.end()) {
        query->sampler = std::move(sampler);
        return true;
    }
    AddSampler(std::move(sampler), location);
    return false;
}

bool RHIResourceSetDesc::SetBuffer(Ref<RHIUniformBuffer> buffer, uint32 location) {
    auto query = std::find_if(mBuffers.begin(), mBuffers.end(), [=](const BufferBinding &b) { return b.location == location; });
    if (query != mBuffers.end()) {
        query->buffer = std::move(buffer);
        return true;
    }
    AddBuffer(std::move(buffer), location);
    return false;
}

void RHIResourceSetDesc::Clear() {
    mTextures.clear();
    mSamplers.clear();
    mBuffers.clear();
}

BRK_NS_END