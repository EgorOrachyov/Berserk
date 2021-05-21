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

#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanCmdList.hpp>

namespace Berserk {
    namespace RHI {

        RefCounted<VertexDeclaration> VulkanDevice::CreateVertexDeclaration(const VertexDeclaration::Desc &desc) {
            return RefCounted<VertexDeclaration>();
        }

        RefCounted<VertexBuffer> VulkanDevice::CreateVertexBuffer(const VertexBuffer::Desc &desc) {
            return RefCounted<VertexBuffer>();
        }

        RefCounted<IndexBuffer> VulkanDevice::CreateIndexBuffer(const IndexBuffer::Desc &desc) {
            return RefCounted<IndexBuffer>();
        }

        RefCounted<UniformBuffer> VulkanDevice::CreateUniformBuffer(const UniformBuffer::Desc &desc) {
            return RefCounted<UniformBuffer>();
        }

        RefCounted<Sampler> VulkanDevice::CreateSampler(const Sampler::Desc &desc) {
            return RefCounted<Sampler>();
        }

        RefCounted<Texture> VulkanDevice::CreateTexture(const Texture::Desc &desc) {
            return RefCounted<Texture>();
        }

        RefCounted<Framebuffer> VulkanDevice::CreateFramebuffer(const Framebuffer::Desc &desc) {
            return RefCounted<Framebuffer>();
        }

        RefCounted<Program> VulkanDevice::CreateProgram(const Program::Desc &desc) {
            return RefCounted<Program>();
        }

        RefCounted<CmdList> VulkanDevice::CreateCmdList() {
            auto commandQueue = VulkanDriver::GetCommandQueue();
            auto& context = VulkanDriver::GetContext();
            auto cmdList = Memory::Make<VulkanCmdList>(std::move(commandQueue), context);
            return RefCounted<CmdList>(cmdList);
        }

        Type VulkanDevice::GetDriverType() const {
            return Type::Vulkan;
        }

        const DeviceCaps &VulkanDevice::GetCaps() const {
            return mCaps;
        }
    }
}