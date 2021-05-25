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

#include <BerserkVulkan/VulkanContext.hpp>
#include <BerserkVulkan/VulkanPipelineCache.hpp>
#include <BerserkVulkan/VulkanFramebufferCache.hpp>

namespace Berserk {
    namespace RHI {


        VulkanContext::VulkanContext(struct VulkanDevice &device) : mDevice(device) {
            mPipelineCache = SharedPtr<VulkanPipelineCache>::Make(mDevice);
            mFboCache = SharedPtr<VulkanFramebufferCache>::Make(mDevice);
        }

        void VulkanContext::BeginScene() {

        }

        void VulkanContext::BeginParallel() {

        }

        void VulkanContext::EndParallel() {

        }

        void VulkanContext::BeginSequence() {

        }

        void VulkanContext::EndSequence() {

        }

        void VulkanContext::UpdateVertexBuffer(const RefCounted<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize,
                                          const void *memory) {

        }

        void VulkanContext::UpdateIndexBuffer(const RefCounted<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize,
                                              const void *memory) {

        }

        void VulkanContext::UpdateUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize,
                                           const void *memory) {

        }

        void VulkanContext::UpdateTexture2D(const RefCounted<Texture> &texture, uint32 mipLevel, const Math::Rect2u &region,
                                       const PixelData &memory) {

        }

        void VulkanContext::UpdateTexture2DArray(const RefCounted<Texture> &texture, uint32 arrayIndex, uint32 mipLevel,
                                                 const Math::Rect2u &region, const PixelData &memory) {

        }

        void VulkanContext::UpdateTextureCube(const RefCounted<Texture> &texture, TextureCubemapFace face, uint32 mipLevel,
                                         const Math::Rect2u &region, const PixelData &memory) {

        }

        void VulkanContext::GenerateMipMaps(const RefCounted<Texture> &texture) {

        }

        void VulkanContext::BeginRenderPass(const RenderPass &renderPass, const RefCounted<Framebuffer> &renderTarget) {

        }

        void VulkanContext::BeginRenderPass(const RenderPass &renderPass, const SharedPtr<Window> &renderTarget) {

        }

        void VulkanContext::BindPipelineState(const PipelineState &pipelineState) {

        }

        void VulkanContext::BindVertexBuffers(const ArrayFixed<RefCounted<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) {

        }

        void VulkanContext::BindIndexBuffer(const RefCounted<IndexBuffer> &buffer, IndexType indexType) {

        }

        void VulkanContext::BindUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 index, uint32 byteOffset, uint32 byteSize) {

        }

        void VulkanContext::BindTexture(const RefCounted<Texture> &texture, uint32 location) {

        }

        void VulkanContext::BindSampler(const RefCounted<Sampler> &sampler, uint32 location) {

        }

        void VulkanContext::Draw(PrimitivesType primType, uint32 verticesCount, uint32 baseVertex, uint32 instancesCount) {

        }

        void VulkanContext::DrawIndexed(PrimitivesType primType, uint32 indexCount, uint32 baseVertex, uint32 baseIndex,
                                        uint32 instanceCount) {

        }

        void VulkanContext::EndRenderPass() {

        }

        void VulkanContext::EndScene() {

        }

        bool VulkanContext::IsInSeparateThreadMode() const {
            return true;
        }

    }
}