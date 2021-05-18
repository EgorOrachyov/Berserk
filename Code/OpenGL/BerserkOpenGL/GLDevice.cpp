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

#include <BerserkOpenGL/GLDevice.hpp>
#include <BerserkOpenGL/GLDriver.hpp>
#include <BerserkOpenGL/GLIndexBuffer.hpp>
#include <BerserkOpenGL/GLProgram.hpp>
#include <BerserkOpenGL/GLSampler.hpp>
#include <BerserkOpenGL/GLTexture.hpp>
#include <BerserkOpenGL/GLUniformBuffer.hpp>
#include <BerserkOpenGL/GLVertexBuffer.hpp>
#include <BerserkOpenGL/GLVertexDeclaration.hpp>
#include <BerserkOpenGL/GLCmdList.hpp>

namespace Berserk {
    namespace RHI {

        template <typename Base>
        struct ResourceProxy final: public Base {
        public:
            template<typename ... TArgs>

            explicit ResourceProxy(TArgs&& ... args) : Base(std::forward<TArgs>(args)...) { }
            ~ResourceProxy() override = default;

            void DeferredInit() {
                GLDriver::GetDeferredResourceContext().SubmitInit([this](){
                    this->Initialize();
                });
            }

        protected:
            void OnReleased() const override {
                GLDriver::GetDeferredResourceContext().SubmitRelease([this](){
                    Memory::Release(this);
                });
            }
        };

        GLDevice::GLDevice() {
            mSupportedShaderLanguages.Add(ShaderLanguage::GLSL410);

#if defined(BERSERK_TARGET_LINUX) || defined(BERSERK_TARGET_WINDOWS)
            mSupportedShaderLanguages.Add(ShaderLanguage::GLSL450);
#endif

            mSupportedTextureFormats.Add(TextureFormat::R8);
            mSupportedTextureFormats.Add(TextureFormat::R8_SNORM);
            mSupportedTextureFormats.Add(TextureFormat::R16);
            mSupportedTextureFormats.Add(TextureFormat::R16_SNORM);
            mSupportedTextureFormats.Add(TextureFormat::RG8);
            mSupportedTextureFormats.Add(TextureFormat::RG8_SNORM);
            mSupportedTextureFormats.Add(TextureFormat::RG16);
            mSupportedTextureFormats.Add(TextureFormat::RG16_SNORM);
            mSupportedTextureFormats.Add(TextureFormat::RGB8);
            mSupportedTextureFormats.Add(TextureFormat::RGB8_SNORM);
            mSupportedTextureFormats.Add(TextureFormat::RGB16_SNORM);
            mSupportedTextureFormats.Add(TextureFormat::RGBA8);
            mSupportedTextureFormats.Add(TextureFormat::RGBA8_SNORM);
            mSupportedTextureFormats.Add(TextureFormat::RGBA16);
            mSupportedTextureFormats.Add(TextureFormat::SRGB8);
            mSupportedTextureFormats.Add(TextureFormat::SRGB8_ALPHA8);
            mSupportedTextureFormats.Add(TextureFormat::R16F);
            mSupportedTextureFormats.Add(TextureFormat::RG16F);
            mSupportedTextureFormats.Add(TextureFormat::RGB16F);
            mSupportedTextureFormats.Add(TextureFormat::RGBA16F);
            mSupportedTextureFormats.Add(TextureFormat::R32F);
            mSupportedTextureFormats.Add(TextureFormat::RG32F);
            mSupportedTextureFormats.Add(TextureFormat::RGB32F);
            mSupportedTextureFormats.Add(TextureFormat::RGBA32F);
            mSupportedTextureFormats.Add(TextureFormat::DEPTH32F);
            mSupportedTextureFormats.Add(TextureFormat::DEPTH32F_STENCIL8);
            mSupportedTextureFormats.Add(TextureFormat::DEPTH24_STENCIL8);
        }

        RefCounted<VertexDeclaration> GLDevice::CreateVertexDeclaration(const VertexDeclaration::Desc &desc) {
            auto vertexDeclaration = Memory::Make<GLVertexDeclaration>(desc);
            return RefCounted<VertexDeclaration>(vertexDeclaration);
        }

        RefCounted<VertexBuffer> GLDevice::CreateVertexBuffer(const VertexBuffer::Desc &desc) {
            using ProxyGLVertexBuffer = ResourceProxy<GLVertexBuffer>;
            auto vertexBuffer = Memory::Make<ProxyGLVertexBuffer>(desc);
            vertexBuffer->DeferredInit();
            return RefCounted<VertexBuffer>(vertexBuffer);
        }

        RefCounted<IndexBuffer> GLDevice::CreateIndexBuffer(const IndexBuffer::Desc &desc) {
            using ProxyGLIndexBuffer = ResourceProxy<GLIndexBuffer>;
            auto indexBuffer = Memory::Make<ProxyGLIndexBuffer>(desc);
            indexBuffer->DeferredInit();
            return RefCounted<IndexBuffer>(indexBuffer);
        }

        RefCounted<UniformBuffer> GLDevice::CreateUniformBuffer(const UniformBuffer::Desc &desc) {
            using ProxyGLUniformBuffer = ResourceProxy<GLUniformBuffer>;
            auto uniformBuffer = Memory::Make<ProxyGLUniformBuffer>(desc);
            uniformBuffer->DeferredInit();
            return RefCounted<UniformBuffer>(uniformBuffer);
        }

        RefCounted<Sampler> GLDevice::CreateSampler(const Sampler::Desc &desc) {
            using ProxyGLSampler = ResourceProxy<GLSampler>;
            auto sampler = Memory::Make<ProxyGLSampler>(desc);
            sampler->DeferredInit();
            return RefCounted<Sampler>(sampler);
        }

        RefCounted<Texture> GLDevice::CreateTexture(const Texture::Desc &desc) {
            using ProxyGLTexture = ResourceProxy<GLTexture>;
            auto texture = Memory::Make<ProxyGLTexture>(desc);
            texture->DeferredInit();
            return RefCounted<Texture>(texture);
        }

        RefCounted<RenderTarget> GLDevice::CreateRenderTarget(const RenderTarget::Desc &desc) {
            return RefCounted<RenderTarget>();
        }

        RefCounted<Program> GLDevice::CreateProgram(const Program::Desc &desc) {
            using ProxyGLProgram = ResourceProxy<GLProgram>;
            auto program = Memory::Make<ProxyGLProgram>(desc);
            program->DeferredInit();
            return RefCounted<Program>(program);
        }

        RefCounted<CmdList> GLDevice::CreateCmdList() {
            auto commandQueue = GLDriver::GetCommandQueue();
            auto& context = GLDriver::GetContext();
            return RefCounted<CmdList>(Memory::Make<GLCmdList>(std::move(commandQueue), context));
        }

        Type GLDevice::GetDriverType() const {
            return Type::OpenGL;
        }
    }
}