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
#include <BerserkOpenGL/GLSampler.hpp>

namespace Berserk {
    namespace RHI {

        RefCounted<VertexDeclaration> GLDevice::CreateVertexDeclaration(const VertexDeclaration::Desc &desc) {
            return RefCounted<VertexDeclaration>();
        }

        RefCounted<VertexBuffer> GLDevice::CreateVertexBuffer(const VertexBuffer::Desc &desc) {
            return RefCounted<VertexBuffer>(Memory::Make<GLVertexBuffer>(desc));
        }

        RefCounted<IndexBuffer> GLDevice::CreateIndexBuffer(const IndexBuffer::Desc &desc) {
            return RefCounted<IndexBuffer>(Memory::Make<GLIndexBuffer>(desc));
        }

        RefCounted<UniformBuffer> GLDevice::CreateUniformBuffer(const UniformBuffer::Desc &desc) {
            return RefCounted<UniformBuffer>(Memory::Make<GLUniformBuffer>(desc));
        }

        RefCounted<Sampler> GLDevice::CreateSampler(const Sampler::Desc &desc) {
            return RefCounted<Sampler>(Memory::Make<GLSampler>(desc));
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

        Type GLDevice::GetDriverType() const {
            return Type::OpenGL;
        }
    }
}