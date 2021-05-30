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

#ifndef BERSERK_GLCONTEXT_HPP
#define BERSERK_GLCONTEXT_HPP

#include <BerserkRHI/RHIContext.hpp>
#include <BerserkOpenGL/GLVaoCache.hpp>
#include <BerserkOpenGL/GLProgram.hpp>
#include <BerserkCore/Templates/HashMap.hpp>
#include <GL/glew.h>

namespace Berserk {
    namespace RHI {

        class GLContext final : public Context {
        public:
            ~GLContext() override = default;

            void BeginScene(const SharedPtr<Window> &window) override;

            void BeginParallel() override;
            void EndParallel() override;
            void BeginSequence() override;
            void EndSequence() override;

            void UpdateVertexBuffer(const RefCounted<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void* memory) override;
            void UpdateIndexBuffer(const RefCounted<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void* memory) override;
            void UpdateUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void* memory) override;
            void UpdateTexture2D(const RefCounted<Texture> &texture, uint32 mipLevel, const Math::Rect2u &region, const PixelData& memory) override;
            void UpdateTexture2DArray(const RefCounted<Texture> &texture, uint32 arrayIndex, uint32 mipLevel, const Math::Rect2u& region, const PixelData& memory) override;
            void UpdateTextureCube(const RefCounted<Texture> &texture, TextureCubemapFace face, uint32 mipLevel, const Math::Rect2u &region, const PixelData &memory) override;
            void GenerateMipMaps(const RefCounted<Texture> &texture) override;

            void BeginRenderPass(const RenderPass &renderPass, const RefCounted<Framebuffer> &renderTarget) override;
            void BeginRenderPass(const RenderPass &renderPass) override;

            void BindPipelineState(const PipelineState &pipelineState) override;
            void BindVertexBuffers(const ArrayFixed<RefCounted<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) override;
            void BindIndexBuffer(const RefCounted <IndexBuffer> &buffer, IndexType indexType) override;
            void BindUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 index, uint32 byteOffset, uint32 byteSize) override;
            void BindTexture(const RefCounted<Texture> &texture, uint32 location) override;
            void BindSampler(const RefCounted<Sampler> &sampler, uint32 location) override;
            void BindTexture(const RefCounted<Texture> &texture, uint32 location, uint32 arrayIndex) override;
            void BindSampler(const RefCounted<Sampler> &sampler, uint32 location, uint32 arrayIndex) override;

            void Draw(uint32 verticesCount, uint32 baseVertex, uint32 instancesCount) override;
            void DrawIndexed(uint32 indexCount, uint32 baseVertex, uint32 baseIndex, uint32 instanceCount) override;

            void EndRenderPass() override;
            void EndScene() override;

            bool IsInSeparateThreadMode() const override;
            void GC();

        private:
            bool mInSceneRendering = false;
            bool mInRenderPass = false;
            bool mPipelineBound = false;
            bool mNeedUpdateVao = true;

            static const uint32 UNUSED_SLOT = 0xffffffff;
            struct Slot { uint32 index = UNUSED_SLOT; };

            HashMap<uint32, Slot> mBoundSlots;                                // Map locations to slots (textures and samplers)
            HashMap<uint32, RefCounted<Texture>> mBoundTextures;              // Map location to bound texture
            HashMap<uint32, RefCounted<Sampler>> mBoundSamplers;              // Map location to bound sampler
            HashMap<uint32, RefCounted<UniformBuffer>> mBoundUniformBuffers;  // Map location to bound buffer
            PipelineState mPipelineState;
            SharedPtr<Window> mWindow;
            GLProgram* mProgram = nullptr;
            GLVaoCache::VaoDescriptor mVaoDesc;
            GLVaoCache mVaoCache;
            GLuint mCurrentVao = 0;
            GLenum mPrimitivesType = GL_NONE;
            GLenum mIndexType = GL_NONE;
            uint32 mNextTextureSlotToBind = 0;
            uint32 mBoundFboColorAttachmentsCount = 0;
            bool mBoundFboHasDepthBuffer = false;
            bool mBoundFboHasStencilBuffer = false;
        };

    }
}

#endif //BERSERK_GLCONTEXT_HPP