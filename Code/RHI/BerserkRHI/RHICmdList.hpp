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

#ifndef BERSERK_RHICMDLIST_HPP
#define BERSERK_RHICMDLIST_HPP

#include <BerserkRHI/RHIContext.hpp>
#include <BerserkRHI/RHIDriver.hpp>
#include <BerserkCore/Templates/SharedPointer.hpp>
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

            void UpdateVertexBuffer(const RefCounted<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([buffer, byteOffset, byteSize, memory](){
                        Driver::GetContext().UpdateVertexBuffer(buffer, byteOffset, byteSize, memory);
                    });
                }
            }

            void UpdateIndexBuffer(const RefCounted<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([buffer, byteOffset, byteSize, memory](){
                        Driver::GetContext().UpdateIndexBuffer(buffer, byteOffset, byteSize, memory);
                    });
                }
            }

            void UpdateUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([buffer, byteOffset, byteSize, memory](){
                        Driver::GetContext().UpdateUniformBuffer(buffer, byteOffset, byteSize, memory);
                    });
                }
            }

            void UpdateTexture2D(const RefCounted<Texture> &texture, uint32 mipLevel, const Math::Rect2u& region, const RefCounted<ReadOnlyMemoryBuffer>& memory) {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([texture, mipLevel, region, memory](){
                        Driver::GetContext().UpdateTexture2D(texture, mipLevel, region, memory);
                    });
                }
            }

            void GenerateMipMaps(const RefCounted<Texture> &texture) {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([texture](){
                        Driver::GetContext().GenerateMipMaps(texture);
                    });
                }
            }

            void BeginRenderPass(const RenderPass& renderPass, const RefCounted<RenderTarget>& renderTarget) {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([renderPass, renderTarget](){
                        Driver::GetContext().BeginRenderPass(renderPass, renderTarget);
                    });
                }
            }

            void BeginRenderPass(const RenderPass& renderPass, const SharedPtr<Window>& renderTarget) {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([renderPass, renderTarget](){
                        Driver::GetContext().BeginRenderPass(renderPass, renderTarget);
                    });
                }
            }

            void BindPipelineState(const PipelineState& pipelineState) {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([pipelineState](){
                        Driver::GetContext().BindPipelineState(pipelineState);
                    });
                }
            }

            void BindVertexBuffers(const ArrayFixed<RefCounted<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([buffers](){
                        Driver::GetContext().BindVertexBuffers(buffers);
                    });
                }
            }

            void BindIndexBuffer(const RefCounted<IndexBuffer> &buffer) {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([buffer](){
                        Driver::GetContext().BindIndexBuffer(buffer);
                    });
                }
            }

            void BindUniformBuffer(const RefCounted<UniformBuffer>& buffer, uint32 index, uint32 byteOffset, uint32 byteSize) {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([buffer, index, byteOffset, byteSize](){
                        Driver::GetContext().BindUniformBuffer(buffer, index, byteOffset, byteSize);
                    });
                }
            }

            void BindTexture(const RefCounted<Texture> &texture, uint32 index) {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([texture, index](){
                        Driver::GetContext().BindTexture(texture, index);
                    });
                }
            }

            void BindSampler(const RefCounted<Sampler> &sampler, uint32 index) {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([sampler, index](){
                        Driver::GetContext().BindSampler(sampler, index);
                    });
                }
            }

            void Draw(uint32 verticesCount, uint32 baseVertex, uint32 instancesCount, uint32 baseInstance) {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([verticesCount, baseVertex, instancesCount, baseInstance](){
                        Driver::GetContext().Draw(verticesCount, baseVertex, instancesCount, baseInstance);
                    });
                }
            }

            void DrawIndexed(uint32 indexCount, uint32 baseIndex, uint32 instanceCount, uint32 baseInstance) {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([indexCount, baseIndex, instanceCount, baseInstance](){
                        Driver::GetContext().DrawIndexed(indexCount, baseIndex, instanceCount, baseInstance);
                    });
                }
            }

            void EndRenderPass() {
                auto& context = Driver::GetContext();

                if (context.IsInSeparateThreadMode()) {
                    mCommandBuffer->Enqueue([](){
                        Driver::GetContext().EndRenderPass();
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