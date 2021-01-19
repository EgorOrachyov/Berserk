/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHICONTEXT_HPP
#define BERSERK_RHICONTEXT_HPP

#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkRHI/RHISampler.hpp>
#include <BerserkRHI/RHIProgram.hpp>
#include <BerserkRHI/RHIVertexBuffer.hpp>
#include <BerserkRHI/RHIIndexBuffer.hpp>
#include <BerserkRHI/RHIUniformBuffer.hpp>
#include <BerserkRHI/RHIVertexDeclaration.hpp>
#include <BerserkRHI/RHITexture.hpp>
#include <BerserkRHI/RHIRenderTarget.hpp>
#include <BerserkRHI/RHIPipelineState.hpp>
#include <BerserkRHI/RHIRenderPass.hpp>
#include <BerserkCore/Platform/Window.hpp>

namespace Berserk {
    namespace RHI {

        /**
         * @brief RHI Context
         *
         * Context object operates on RHI thread and provides rendering/resource update functionality.
         * Commands, captured in the multi-threaded command lists, are executed on the RHI thread
         * via this context object.
         */
        class Context {
        public:
            virtual ~Context() = default;

            virtual void UpdateVertexBuffer(const Ref<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<MemoryBuffer> &memory) = 0;

            virtual void UpdateIndexBuffer(const Ref<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<MemoryBuffer> &memory) = 0;

            virtual void UpdateUniformBuffer(const Ref<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<MemoryBuffer> &memory) = 0;

            virtual void UpdateTexture2D(const Ref<Texture> &texture, uint32 mipLevel, const Math::Rect2u& region, const Ref<MemoryBuffer>& memory) = 0;

            virtual void GenerateMipMaps(const Ref<Texture> &texture) = 0;

            virtual void BeginRenderPass(const RenderPass& renderPass, const Ref<RenderTarget>& renderTarget) = 0;

            virtual void BeginRenderPass(const RenderPass& renderPass, const Ref<Platform::Window>& renderTarget) = 0;

            virtual void BindPipelineState(const PipelineState& pipelineState) = 0;

            virtual void BindVertexBuffers(const ArrayFixed<Ref<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) = 0;

            virtual void BindIndexBuffer(const Ref<IndexBuffer> &buffer) = 0;

            virtual void BindUniformBuffer(const Ref<UniformBuffer>& buffer, uint32 index, uint32 byteOffset, uint32 byteSize) = 0;

            virtual void BindTexture(const Ref<Texture> &texture, uint32 index) = 0;

            virtual void BindSampler(const Ref<Sampler> &sampler, uint32 index) = 0;

            virtual void Draw(uint32 verticesCount, uint32 baseVertex, uint32 instancesCount, uint32 baseInstance) = 0;

            virtual void DrawIndexed(uint32 indexCount, uint32 baseIndex, uint32 instanceCount, uint32 baseInstance) = 0;

            virtual void EndRenderPass() = 0;
        };

    }
}

#endif //BERSERK_RHICONTEXT_HPP