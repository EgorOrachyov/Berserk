/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIDRIVER_HPP
#define BERSERK_RHIDRIVER_HPP

#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkRHI/RHISampler.hpp>
#include <BerserkRHI/RHIProgram.hpp>
#include <BerserkRHI/RHIVertexBuffer.hpp>
#include <BerserkRHI/RHIIndexBuffer.hpp>
#include <BerserkRHI/RHIUniformBuffer.hpp>
#include <BerserkRHI/RHIVertexDeclaration.hpp>
#include <BerserkRHI/RHITexture.hpp>
#include <BerserkRHI/RHIRenderTarget.hpp>

namespace Berserk {
    namespace RHI {

        /**
         * @brief RHI driver
         *
         * Driver object exposes common object creation api.
         *
         * Driver objects can be safely created from any thread. Create functions
         * immediately return the created object reference, but actual object creation
         * on the GPU driver side is deferred, until the RHI execution thread reaches
         * object creation and does it its side.
         *
         * From user side, created objects can be safely and immediately used in
         * the drawing and update api via command buffers.
         */
        class Driver {
        public:
            virtual ~Driver() = default;

            virtual Ref<VertexDeclaration> CreateVertexDeclaration(const VertexDeclaration::Desc& desc) = 0;

            virtual Ref<VertexBuffer> CreateVertexBuffer(const VertexBuffer::Desc& desc) = 0;

            virtual Ref<IndexBuffer> CreateIndexBuffer(const IndexBuffer::Desc& desc) = 0;

            virtual Ref<UniformBuffer> CreateUniformBuffer(const UniformBuffer::Desc& desc) = 0;

            virtual Ref<Sampler> CreateSampler(const Sampler::Desc& desc) = 0;

            virtual Ref<Texture> CreateTexture(const Texture::Desc& desc) = 0;

            virtual Ref<RenderTarget> CreateRenderTarget(const RenderTarget::Desc& desc) = 0;

            virtual Ref<Program> CreateProgram(const Program::Desc& desc) = 0;

            virtual const Array<TextureFormat> &GetSupportedFormats() const;

            virtual const Array<ShaderLanguage> &GetSupportedShaderLanguages() const;

            virtual Type GetDriverType() const = 0;

        protected:
            Array<TextureFormat> mSupportedTextureFormats;
            Array<ShaderLanguage> mSupportedShaderLanguages;
        };


    }
}

#endif //BERSERK_RHIDRIVER_HPP