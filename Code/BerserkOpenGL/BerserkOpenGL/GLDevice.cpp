/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkOpenGL/GLDevice.hpp>
#include <BerserkOpenGL/GLVertexBuffer.hpp>
#include <BerserkOpenGL/GLIndexBuffer.hpp>
#include <BerserkOpenGL/GLUniformBuffer.hpp>

namespace Berserk {
    namespace RHI {

        Ref<VertexDeclaration> GLDevice::CreateVertexDeclaration(const VertexDeclaration::Desc &desc) {
            return Ref<VertexDeclaration>();
        }

        Ref<VertexBuffer> GLDevice::CreateVertexBuffer(const VertexBuffer::Desc &desc) {
            return Ref<VertexBuffer>(Platform::Memory::Make<GLVertexBuffer>(desc));
        }

        Ref<IndexBuffer> GLDevice::CreateIndexBuffer(const IndexBuffer::Desc &desc) {
            return Ref<IndexBuffer>(Platform::Memory::Make<GLIndexBuffer>(desc));
        }

        Ref<UniformBuffer> GLDevice::CreateUniformBuffer(const UniformBuffer::Desc &desc) {
            return Ref<UniformBuffer>(Platform::Memory::Make<GLUniformBuffer>(desc));
        }

        Ref<Sampler> GLDevice::CreateSampler(const Sampler::Desc &desc) {
            return Ref<Sampler>();
        }

        Ref<Texture> GLDevice::CreateTexture(const Texture::Desc &desc) {
            return Ref<Texture>();
        }

        Ref<RenderTarget> GLDevice::CreateRenderTarget(const RenderTarget::Desc &desc) {
            return Ref<RenderTarget>();
        }

        Ref<Program> GLDevice::CreateProgram(const Program::Desc &desc) {
            return Ref<Program>();
        }

        Type GLDevice::GetDriverType() const {
            return Type::OpenGL;
        }
    }
}