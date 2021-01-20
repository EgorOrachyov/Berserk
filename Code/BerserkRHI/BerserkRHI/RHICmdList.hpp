/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHICMDLIST_HPP
#define BERSERK_RHICMDLIST_HPP

#include <BerserkRHI/RHIContext.hpp>
#include <BerserkRHI/RHI.hpp>
#include <BerserkCore/Threading/CommandBuffer.hpp>

namespace Berserk {
    namespace RHI {

        /**
         * @brief Commands List
         *
         * Commands list allows to capture rendering and update commands for later
         * execution on the RHI thread. This class allows to organize async
         * rendering, where commands capturing and its actual execution are done
         * by different threads at the same or not time.
         *
         * @note Access to this object must be synchronized.
         */
        class CmdList {
        public:
            CmdList();
            ~CmdList();

            CmdList(CmdList&& other) = delete;
            CmdList(const CmdList& other) = delete;

            void UpdateVertexBuffer(const Ref<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<MemoryBuffer> &memory) {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([buffer, byteOffset, byteSize, memory](){
                        RHI::GetContext().UpdateVertexBuffer(buffer, byteOffset, byteSize, memory);
                    });
                }
            }

            void UpdateIndexBuffer(const Ref<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<MemoryBuffer> &memory) {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([buffer, byteOffset, byteSize, memory](){
                        RHI::GetContext().UpdateIndexBuffer(buffer, byteOffset, byteSize, memory);
                    });
                }
            }

            void UpdateUniformBuffer(const Ref<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<MemoryBuffer> &memory) {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([buffer, byteOffset, byteSize, memory](){
                        RHI::GetContext().UpdateUniformBuffer(buffer, byteOffset, byteSize, memory);
                    });
                }
            }

            void UpdateTexture2D(const Ref<Texture> &texture, uint32 mipLevel, const Math::Rect2u& region, const Ref<MemoryBuffer>& memory) {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([texture, mipLevel, region, memory](){
                        RHI::GetContext().UpdateTexture2D(texture, mipLevel, region, memory);
                    });
                }
            }

            void GenerateMipMaps(const Ref<Texture> &texture) {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([texture](){
                        RHI::GetContext().GenerateMipMaps(texture);
                    });
                }
            }

            void BeginRenderPass(const RenderPass& renderPass, const Ref<RenderTarget>& renderTarget) {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([renderPass, renderTarget](){
                        RHI::GetContext().BeginRenderPass(renderPass, renderTarget);
                    });
                }
            }

            void BeginRenderPass(const RenderPass& renderPass, const Ref<Platform::Window>& renderTarget) {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([renderPass, renderTarget](){
                        RHI::GetContext().BeginRenderPass(renderPass, renderTarget);
                    });
                }
            }

            void BindPipelineState(const PipelineState& pipelineState) {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([pipelineState](){
                        RHI::GetContext().BindPipelineState(pipelineState);
                    });
                }
            }

            void BindVertexBuffers(const ArrayFixed<Ref<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([buffers](){
                        RHI::GetContext().BindVertexBuffers(buffers);
                    });
                }
            }

            void BindIndexBuffer(const Ref<IndexBuffer> &buffer) {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([buffer](){
                        RHI::GetContext().BindIndexBuffer(buffer);
                    });
                }
            }

            void BindUniformBuffer(const Ref<UniformBuffer>& buffer, uint32 index, uint32 byteOffset, uint32 byteSize) {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([buffer, index, byteOffset, byteSize](){
                        RHI::GetContext().BindUniformBuffer(buffer, index, byteOffset, byteSize);
                    });
                }
            }

            void BindTexture(const Ref<Texture> &texture, uint32 index) {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([texture, index](){
                        RHI::GetContext().BindTexture(texture, index);
                    });
                }
            }

            void BindSampler(const Ref<Sampler> &sampler, uint32 index) {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([sampler, index](){
                        RHI::GetContext().BindSampler(sampler, index);
                    });
                }
            }

            void Draw(uint32 verticesCount, uint32 baseVertex, uint32 instancesCount, uint32 baseInstance) {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([verticesCount, baseVertex, instancesCount, baseInstance](){
                        RHI::GetContext().Draw(verticesCount, baseVertex, instancesCount, baseInstance);
                    });
                }
            }

            void DrawIndexed(uint32 indexCount, uint32 baseIndex, uint32 instanceCount, uint32 baseInstance) {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([indexCount, baseIndex, instanceCount, baseInstance](){
                        RHI::GetContext().DrawIndexed(indexCount, baseIndex, instanceCount, baseInstance);
                    });
                }
            }

            void EndRenderPass() {
                auto& context = RHI::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([](){
                        RHI::GetContext().EndRenderPass();
                    });
                }
            }

            /**
             * @brief Commit captured commands to the RHI thread.
             *
             * This command commits captured rendering and update commands for
             * the execution to the RHI thread. This commands list will be placed into
             * queue, and the later fetched by RHI thread and executed on its side.
             * Actual execution will happen as soon as RHI thread reaches this
             * commands list. It might happen in this or in the next logical frame processing.
             *
             * @note This function might be called from any thread.
             */
            void Commit();

        protected:
            CommandBuffer* mCommandBuffer = nullptr;
        };

    }
}

#endif //BERSERK_RHICMDLIST_HPP