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

#ifndef BERSERK_VULKANDEVICE_HPP
#define BERSERK_VULKANDEVICE_HPP

#include <BerserkRHI/RHIDevice.hpp>

namespace Berserk {
    namespace RHI {

        class VulkanDevice final: public Device {
        public:
            ~VulkanDevice() override = default;

            RefCounted<VertexDeclaration> CreateVertexDeclaration(const VertexDeclaration::Desc &desc) override;

            RefCounted<VertexBuffer> CreateVertexBuffer(const VertexBuffer::Desc &desc) override;

            RefCounted<IndexBuffer> CreateIndexBuffer(const IndexBuffer::Desc &desc) override;

            RefCounted<UniformBuffer> CreateUniformBuffer(const UniformBuffer::Desc &desc) override;

            RefCounted<Sampler> CreateSampler(const Sampler::Desc &desc) override;

            RefCounted<Texture> CreateTexture(const Texture::Desc &desc) override;

            RefCounted<Framebuffer> CreateFramebuffer(const Framebuffer::Desc &desc) override;

            RefCounted<Program> CreateProgram(const Program::Desc &desc) override;

            RefCounted<CmdList> CreateCmdList() override;

            Type GetDriverType() const override;

            const DeviceCaps &GetCaps() const override;

        private:
            DeviceCaps mCaps;
        };

    }
}

#endif //BERSERK_VULKANDEVICE_HPP