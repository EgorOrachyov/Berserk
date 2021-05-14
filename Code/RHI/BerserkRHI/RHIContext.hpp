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
#include <BerserkCore/Templates/SharedPointer.hpp>

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

            virtual void UpdateVertexBuffer(const RefCounted<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) = 0;

            virtual void UpdateIndexBuffer(const RefCounted<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) = 0;

            virtual void UpdateUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) = 0;

            virtual void UpdateTexture2D(const RefCounted<Texture> &texture, uint32 mipLevel, const Math::Rect2u& region, const RefCounted<ReadOnlyMemoryBuffer>& memory) = 0;

            virtual void GenerateMipMaps(const RefCounted<Texture> &texture) = 0;

            virtual void BeginRenderPass(const RenderPass& renderPass, const RefCounted<RenderTarget>& renderTarget) = 0;

            virtual void BeginRenderPass(const RenderPass& renderPass, const SharedPtr<Window>& renderTarget) = 0;

            virtual void BindPipelineState(const PipelineState& pipelineState) = 0;

            virtual void BindVertexBuffers(const ArrayFixed<RefCounted<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) = 0;

            virtual void BindIndexBuffer(const RefCounted<IndexBuffer> &buffer) = 0;

            virtual void BindUniformBuffer(const RefCounted<UniformBuffer>& buffer, uint32 index, uint32 byteOffset, uint32 byteSize) = 0;

            virtual void BindTexture(const RefCounted<Texture> &texture, uint32 index) = 0;

            virtual void BindSampler(const RefCounted<Sampler> &sampler, uint32 index) = 0;

            virtual void Draw(uint32 verticesCount, uint32 baseVertex, uint32 instancesCount, uint32 baseInstance) = 0;

            virtual void DrawIndexed(uint32 indexCount, uint32 baseIndex, uint32 instanceCount, uint32 baseInstance) = 0;

            virtual void EndRenderPass() = 0;

            virtual bool IsInSeparateThreadMode() const = 0;
        };

    }
}

#endif //BERSERK_RHICONTEXT_HPP