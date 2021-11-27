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

#ifndef BERSERK_RHIDEVICE_HPP
#define BERSERK_RHIDEVICE_HPP

#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkRHI/RHISampler.hpp>
#include <BerserkRHI/RHIProgram.hpp>
#include <BerserkRHI/RHIVertexBuffer.hpp>
#include <BerserkRHI/RHIIndexBuffer.hpp>
#include <BerserkRHI/RHIUniformBuffer.hpp>
#include <BerserkRHI/RHIVertexDeclaration.hpp>
#include <BerserkRHI/RHITexture.hpp>
#include <BerserkRHI/RHIFramebuffer.hpp>
#include <BerserkRHI/RHICmdList.hpp>
#include <BerserkCore/Math/TMatMxN.hpp>

namespace Berserk {
    namespace RHI {

        /**
         * @brief RHI device
         *
         * Device object exposes common object creation api.
         *
         * Device provided objects can be safely created from any thread. Create functions
         * immediately return the created object reference, but actual object creation
         * on the GPU side is deferred, until the RHI execution thread reaches
         * object creation and does it its side.
         *
         * From user side, created objects can be safely and immediately used in
         * the drawing and update api via command buffers.
         */
        class Device {
        public:
            virtual ~Device() = default;

            virtual RcPtr<VertexDeclaration> CreateVertexDeclaration(const VertexDeclaration::Desc& desc) = 0;

            virtual RcPtr<VertexBuffer> CreateVertexBuffer(const VertexBuffer::Desc& desc) = 0;

            virtual RcPtr<IndexBuffer> CreateIndexBuffer(const IndexBuffer::Desc& desc) = 0;

            virtual RcPtr<UniformBuffer> CreateUniformBuffer(const UniformBuffer::Desc& desc) = 0;

            virtual RcPtr<Sampler> CreateSampler(const Sampler::Desc& desc) = 0;

            virtual RcPtr<Texture> CreateTexture(const Texture::Desc& desc) = 0;

            virtual RcPtr<Framebuffer> CreateFramebuffer(const Framebuffer::Desc& desc) = 0;

            virtual RcPtr<Program> CreateProgram(const Program::Desc& desc) = 0;

            virtual RcPtr<CmdList> CreateCmdList() = 0;

            virtual const Array<TextureFormat> &GetSupportedFormats() const;

            virtual const Array<ShaderLanguage> &GetSupportedShaderLanguages() const;

            virtual Type GetDriverType() const = 0;

            virtual const DeviceCaps &GetCaps() const = 0;

            virtual const Math::Mat4x4f &GetClipMatrix() const = 0;

        protected:
            Array<TextureFormat> mSupportedTextureFormats;
            Array<ShaderLanguage> mSupportedShaderLanguages;
        };


    }
}

#endif //BERSERK_RHIDEVICE_HPP