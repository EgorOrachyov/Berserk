/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_GLDEVICE_HPP
#define BERSERK_GLDEVICE_HPP

#include <BerserkRHI/RHIDevice.hpp>

namespace Berserk {
    namespace RHI {

        class GLDevice final : public Device {
        public:
            ~GLDevice() override = default;

            Ref<VertexDeclaration> CreateVertexDeclaration(const VertexDeclaration::Desc &desc) override;

            Ref<VertexBuffer> CreateVertexBuffer(const VertexBuffer::Desc &desc) override;

            Ref<IndexBuffer> CreateIndexBuffer(const IndexBuffer::Desc &desc) override;

            Ref<UniformBuffer> CreateUniformBuffer(const UniformBuffer::Desc &desc) override;

            Ref<Sampler> CreateSampler(const Sampler::Desc &desc) override;

            Ref<Texture> CreateTexture(const Texture::Desc &desc) override;

            Ref<RenderTarget> CreateRenderTarget(const RenderTarget::Desc &desc) override;

            Ref<Program> CreateProgram(const Program::Desc &desc) override;

            Type GetDriverType() const override;

        protected:

        };

    }
}



#endif //BERSERK_GLDEVICE_HPP
