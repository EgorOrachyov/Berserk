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
#include <BerserkRHI/RHIFramebuffer.hpp>
#include <BerserkRHI/RHIPipelineState.hpp>
#include <BerserkRHI/RHIRenderPass.hpp>
#include <BerserkCore/Image/PixelData.hpp>
#include <BerserkCore/Platform/Window.hpp>
#include <BerserkCore/Templates/SmartPointer.hpp>

namespace Berserk {
    namespace RHI {

        /**
         * @brief RHI Context
         *
         * Context object operates on RHI thread and provides rendering/resource update functionality.
         * Commands, captured in the multi-threaded command lists, are executed on the RHI thread
         * via this context object.
         */
        class Context
                {
        public:
            virtual ~Context() = default;

            /**
             * @note RHI-Thread only
             */
            virtual void BeginScene() = 0;

            /**
             * Start section where all render passes (following blocks) are executed in parallel GPU queues.
             * This section is ordered withing Context execution stages.
             *
             *          |
             *          BeginParallel()
             *       ___|.....____
             *       |     |     |
             *    [cmds][cmds][cmds]
             *       |     |     |
             *       ----.....----
             *          |
             *          EndParallel()
             *
             * @note Call EndParallel() to close this parallel section.
             * @note RHI-Thread only
             */
            virtual void BeginParallel() = 0;

            /**
             * End section where all render passes (following blocks) are executed in parallel GPU queues.
             * This section is ordered withing Context execution stages.
             *
             *          |
             *          BeginParallel()
             *       ___|.....____
             *       |     |     |
             *    [cmds][cmds][cmds]
             *       |     |     |
             *       ----.....----
             *          |
             *          EndParallel()
             *
             * @note Call BeginSequence() before to open this parallel section.
             * @note RHI-Thread only
             */
            virtual void EndParallel() = 0;

            /**
             * Begin section where al render passes (following blocks) are executed in sequence on single GPU queue.
             * This section is ordered withing Context execution stages.
             *
             *          |
             *          BeginSequence()
             *          |
             *         [cmds]
             *         [cmds]
             *         [cmds]
             *          |
             *          EndSequence()
             *          |
             *
             * @note Call EndSequence() to close this sequenced section.
             * @note RHI-Thread only
             */
            virtual void BeginSequence() = 0;

            /**
             * End section where al render passes (following blocks) are executed in sequence on single GPU queue.
             * This section is ordered withing Context execution stages.
             *
             *          |
             *          BeginSequence()
             *          |
             *         [cmds]
             *         [cmds]
             *         [cmds]
             *          |
             *          EndSequence()
             *          |
             *
             * @note Call BeginSequence() before to open this sequenced section.
             * @note RHI-Thread only
             */
            virtual void EndSequence() = 0;

            /**
             * @note RHI-Thread only
             *
             * @param buffer
             * @param byteOffset
             * @param byteSize
             * @param memory
             */
            virtual void UpdateVertexBuffer(const RefCounted<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void* memory) = 0;

            /**
             * @note RHI-Thread only
             *
             * @param buffer
             * @param byteOffset
             * @param byteSize
             * @param memory
             */
            virtual void UpdateIndexBuffer(const RefCounted<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void* memory) = 0;

            /**
             * @note RHI-Thread only
             *
             * @param buffer
             * @param byteOffset
             * @param byteSize
             * @param memory
             */
            virtual void UpdateUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void* memory) = 0;

            /**
             * @note RHI-Thread only
             *
             * @param texture
             * @param mipLevel
             * @param region
             * @param memory
             */
            virtual void UpdateTexture2D(const RefCounted<Texture> &texture, uint32 mipLevel, const Math::Rect2u& region, const PixelData& memory) = 0;

            /**
             *
             * @param texture
             * @param arrayIndex
             * @param mipLevel
             * @param region
             * @param memory
             */
            virtual void UpdateTexture2DArray(const RefCounted<Texture> &texture, uint32 arrayIndex, uint32 mipLevel, const Math::Rect2u& region, const PixelData& memory) = 0;

            /**
             *
             * @param texture
             * @param face
             * @param mipLevel
             * @param region
             * @param memory
             */
            virtual void UpdateTextureCube(const RefCounted<Texture> &texture, TextureCubemapFace face, uint32 mipLevel, const Math::Rect2u& region, const PixelData& memory) = 0;

            /**
             * Generate mipmaps for specified textures.
             * Uses data provided from level-0 as base to generate images for the rest of the levels.
             *
             * @note Can generate mipmaps only for textures max available mips count (@see PixelUtil::GetMaxMipsCount())
             * @note RHI-Thread only
             *
             * @param texture Texture to generate mipmaps on
             */
            virtual void GenerateMipMaps(const RefCounted<Texture> &texture) = 0;

            /**
             * @note RHI-Thread only
             *
             * @param renderPass
             * @param renderTarget
             */
            virtual void BeginRenderPass(const RenderPass& renderPass, const RefCounted<Framebuffer>& renderTarget) = 0;

            /**
             * @note RHI-Thread only
             *
             * @param renderPass
             * @param renderTarget
             */
            virtual void BeginRenderPass(const RenderPass& renderPass, const SharedPtr<Window>& renderTarget) = 0;

            /**
             * @note RHI-Thread only
             *
             * @param pipelineState
             */
            virtual void BindPipelineState(const PipelineState& pipelineState) = 0;

            /**
             * @note RHI-Thread only
             *
             * @param buffers
             */
            virtual void BindVertexBuffers(const ArrayFixed<RefCounted<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) = 0;

            /**
             * @note RHI-Thread only
             *
             * @param buffer
             * @param indexType
             */
            virtual void BindIndexBuffer(const RefCounted<IndexBuffer> &buffer, IndexType indexType) = 0;

            /**
             * @note RHI-Thread only
             *
             * @param buffer
             * @param index
             * @param byteOffset
             * @param byteSize
             */
            virtual void BindUniformBuffer(const RefCounted<UniformBuffer>& buffer, uint32 index, uint32 byteOffset, uint32 byteSize) = 0;

            /**
             * @note RHI-Thread only
             *
             * @param texture
             * @param slot
             */
            virtual void BindTexture(const RefCounted<Texture> &texture, uint32 location) = 0;

            /**
             * @note RHI-Thread only
             *
             * @param sampler
             * @param slot
             */
            virtual void BindSampler(const RefCounted<Sampler> &sampler, uint32 location) = 0;

            /**
             * @note RHI-Thread only
             *
             * @param primType
             * @param verticesCount
             * @param baseVertex
             * @param instancesCount
             */
            virtual void Draw(PrimitivesType primType, uint32 verticesCount, uint32 baseVertex, uint32 instancesCount) = 0;

            /**
             * @note RHI-Thread only
             *
             * @param primType
             * @param indexCount
             * @param baseVertex
             * @param baseIndex
             * @param instanceCount
             */
            virtual void DrawIndexed(PrimitivesType primType, uint32 indexCount, uint32 baseVertex, uint32 baseIndex, uint32 instanceCount) = 0;

            /**
             * @note RHI-Thread only
             */
            virtual void EndRenderPass() = 0;

            /**
             * @note RHI-Thread only
             */
            virtual void EndScene() = 0;

            /**
             * @note Any-Thread
             *
             * @return
             */
            virtual bool IsInSeparateThreadMode() const = 0;
        };

    }
}

#endif //BERSERK_RHICONTEXT_HPP