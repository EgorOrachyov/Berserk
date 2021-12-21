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

#ifndef BERSERK_GLRESOURCESET_HPP
#define BERSERK_GLRESOURCESET_HPP

#include <rhi/RHIResourceSet.hpp>
#include <rhi/opengl/GLDefs.hpp>
#include <rhi/opengl/GLShader.hpp>

#include <unordered_map>

BRK_NS_BEGIN

/**
 * @addtogroup opengl
 * @{
 */

/**
 * @class GLResourceBindingState
 * @brief Auxiliary class used to bind textures and samplers to pipeline
 */
class GLResourceBindingState {
public:
    BRK_API uint32 GetSlot(uint32 location);
    BRK_API void Clear();

private:
    static const uint32 UNUSED_SLOT = 0xffffffff;
    struct Slot {
        uint32 index = UNUSED_SLOT;
    };

private:
    std::unordered_map<uint32, Slot> mBoundSlots;
    uint32 mNextSlot = 0;
};

/**
 * @class GLResourceSet
 * @brief GL set of pipeline resources
 */
class GLResourceSet final : public RHIResourceSet {
public:
    using TextureBinding = RHIResourceSetDesc::TextureBinding;
    using SamplerBinding = RHIResourceSetDesc::SamplerBinding;
    using BufferBinding = RHIResourceSetDesc::BufferBinding;

    BRK_API explicit GLResourceSet(const RHIResourceSetDesc &desc);
    BRK_API ~GLResourceSet() override = default;

    BRK_API void Update(const RHIResourceSetDesc &desc);
    BRK_API void Bind(GLResourceBindingState &state, const Ref<GLShader> &shader) const;

    const std::vector<TextureBinding> &GetTextures() const { return mTextures; }
    const std::vector<SamplerBinding> &GetSamplers() const { return mSamplers; }
    const std::vector<BufferBinding> &GetBuffers() const { return mBuffers; }

private:
    std::vector<TextureBinding> mTextures;
    std::vector<SamplerBinding> mSamplers;
    std::vector<BufferBinding> mBuffers;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_GLRESOURCESET_HPP
