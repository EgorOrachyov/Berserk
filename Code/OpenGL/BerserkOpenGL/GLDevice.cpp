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
#include <BerserkOpenGL/GLVertexBuffer.hpp>
#include <BerserkOpenGL/GLIndexBuffer.hpp>
#include <BerserkOpenGL/GLUniformBuffer.hpp>
#include <BerserkOpenGL/GLSampler.hpp>
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
                GLDriver::GetDeferredResourceContext().SubmitRelease([this](){
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

        RefCounted<VertexDeclaration> GLDevice::CreateVertexDeclaration(const VertexDeclaration::Desc &desc) {
            return RefCounted<VertexDeclaration>();
        }

        RefCounted<VertexBuffer> GLDevice::CreateVertexBuffer(const VertexBuffer::Desc &desc) {
            using ProxyGLVertexBuffer = ResourceProxy<GLVertexBuffer>;
            auto vertexBuffer = Memory::Make<ProxyGLVertexBuffer>(desc);
            vertexBuffer->DeferredInit();
            return RefCounted<VertexBuffer>(vertexBuffer, RefCountedBoxing::AddRefs);
        }

        RefCounted<IndexBuffer> GLDevice::CreateIndexBuffer(const IndexBuffer::Desc &desc) {
            using ProxyGLIndexBuffer = ResourceProxy<GLIndexBuffer>;
            auto indexBuffer = Memory::Make<ProxyGLIndexBuffer>(desc);
            indexBuffer->DeferredInit();
            return RefCounted<IndexBuffer>(indexBuffer, RefCountedBoxing::AddRefs);
        }

        RefCounted<UniformBuffer> GLDevice::CreateUniformBuffer(const UniformBuffer::Desc &desc) {
            using ProxyGLUniformBuffer = ResourceProxy<GLUniformBuffer>;
            auto uniformBuffer = Memory::Make<ProxyGLUniformBuffer>(desc);
            uniformBuffer->DeferredInit();
            return RefCounted<UniformBuffer>(uniformBuffer, RefCountedBoxing::AddRefs);
        }

        RefCounted<Sampler> GLDevice::CreateSampler(const Sampler::Desc &desc) {
            using ProxyGLSampler = ResourceProxy<GLSampler>;
            auto sampler = Memory::Make<ProxyGLSampler>(desc);
            sampler->DeferredInit();
            return RefCounted<Sampler>(sampler, RefCountedBoxing::AddRefs);
        }

        RefCounted<Texture> GLDevice::CreateTexture(const Texture::Desc &desc) {
            return RefCounted<Texture>();
        }

        RefCounted<RenderTarget> GLDevice::CreateRenderTarget(const RenderTarget::Desc &desc) {
            return RefCounted<RenderTarget>();
        }

        RefCounted<Program> GLDevice::CreateProgram(const Program::Desc &desc) {
            return RefCounted<Program>();
        }

        RefCounted<CmdList> GLDevice::CreateCmdList() {
            auto commandQueue = GLDriver::GetCommandQueue();
            auto& context = GLDriver::GetContext();
            return RefCounted<CmdList>(Memory::Make<GLCmdList>(std::move(commandQueue), context), RefCountedBoxing::AddRefs);
        }

        Type GLDevice::GetDriverType() const {
            return Type::OpenGL;
        }
    }
}