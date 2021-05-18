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
#include <BerserkCore/Templates/SharedPointer.hpp>
#include <BerserkCore/Templates/AsyncCommandQueue.hpp>

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
        class CmdList: public Resource {
        public:
            CmdList(AsyncCommandQueue<> &&commandQueue, Context& context)
                : mCommandQueue(std::move(commandQueue)), mContext(&context) {}

            ~CmdList() override = default;

            void BeginScene() {
                assert(!mBeginCalled);

                mBeginCalled = true;
                auto context = mContext;

                mCommandQueue.Submit([context](){
                    context->BeginScene();
                });
            }

            void UpdateVertexBuffer(const RefCounted<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) {
                assert(!mRenderPass);
                assert(buffer);

                auto context = mContext;

                mCommandQueue.Submit([context, buffer, byteOffset, byteSize, memory](){
                    context->UpdateVertexBuffer(buffer, byteOffset, byteSize, memory);
                });
            }

            void UpdateIndexBuffer(const RefCounted<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) {
                assert(!mRenderPass);
                assert(buffer);

                auto context = mContext;

                mCommandQueue.Submit([context, buffer, byteOffset, byteSize, memory](){
                    context->UpdateIndexBuffer(buffer, byteOffset, byteSize, memory);
                });
            }

            void UpdateUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) {
                assert(!mRenderPass);
                assert(buffer);

                auto context = mContext;

                mCommandQueue.Submit([context, buffer, byteOffset, byteSize, memory](){
                    context->UpdateUniformBuffer(buffer, byteOffset, byteSize, memory);
                });
            }

            void UpdateTexture2D(const RefCounted<Texture> &texture, uint32 mipLevel, const Math::Rect2u& region, const RefCounted<PixelData>& memory) {
                assert(!mRenderPass);
                assert(texture);

                auto context = mContext;

                mCommandQueue.Submit([context, texture, mipLevel, region, memory](){
                    context->UpdateTexture2D(texture, mipLevel, region, memory);
                });
            }

            void GenerateMipMaps(const RefCounted<Texture> &texture) {
                assert(!mRenderPass);
                assert(texture);

                auto context = mContext;

                mCommandQueue.Submit([context, texture](){
                    context->GenerateMipMaps(texture);
                });
            }

            void BeginRenderPass(const RenderPass& renderPass, const RefCounted<Framebuffer>& renderTarget) {
                assert(mBeginCalled);
                assert(!mRenderPass);
                assert(renderTarget);

                mRenderPass = true;
                auto context = mContext;

                mCommandQueue.Submit([context, renderPass, renderTarget](){
                    context->BeginRenderPass(renderPass, renderTarget);
                });
            }

            void BeginRenderPass(const RenderPass& renderPass, const SharedPtr<Window>& renderTarget) {
                assert(mBeginCalled);
                assert(!mRenderPass);
                assert(renderTarget);

                mRenderPass = true;
                auto context = mContext;

                mCommandQueue.Submit([context, renderPass, renderTarget](){
                    context->BeginRenderPass(renderPass, renderTarget);
                });
            }

            void BindPipelineState(const PipelineState& pipelineState) {
                assert(mBeginCalled);
                assert(mRenderPass);

                auto context = mContext;

                mCommandQueue.Submit([context, pipelineState](){
                    context->BindPipelineState(pipelineState);
                });
            }

            void BindVertexBuffers(const ArrayFixed<RefCounted<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) {
                assert(mBeginCalled);
                assert(mRenderPass);

                auto context = mContext;

                mCommandQueue.Submit([context, buffers](){
                    context->BindVertexBuffers(buffers);
                });
            }

            void BindIndexBuffer(const RefCounted<IndexBuffer> &buffer, IndexType indexType) {
                assert(mBeginCalled);
                assert(mRenderPass);
                assert(buffer);

                auto context = mContext;

                mCommandQueue.Submit([context, buffer, indexType](){
                    context->BindIndexBuffer(buffer, indexType);
                });
            }

            void BindUniformBuffer(const RefCounted<UniformBuffer>& buffer, uint32 index, uint32 byteOffset, uint32 byteSize) {
                assert(mBeginCalled);
                assert(mRenderPass);
                assert(buffer);

                auto context = mContext;

                mCommandQueue.Submit([context, buffer, index, byteOffset, byteSize](){
                    context->BindUniformBuffer(buffer, index, byteOffset, byteSize);
                });
            }

            void BindTexture(const RefCounted<Texture> &texture, uint32 index) {
                assert(mBeginCalled);
                assert(mRenderPass);
                assert(texture);

                auto context = mContext;

                mCommandQueue.Submit([context, texture, index](){
                    context->BindTexture(texture, index);
                });
            }

            void BindSampler(const RefCounted<Sampler> &sampler, uint32 index) {
                assert(mBeginCalled);
                assert(mRenderPass);
                assert(sampler);

                auto context = mContext;

                mCommandQueue.Submit([context, sampler, index](){
                    context->BindSampler(sampler, index);
                });
            }

            void Draw(uint32 verticesCount, uint32 baseVertex, uint32 instancesCount) {
                assert(mBeginCalled);
                assert(mRenderPass);

                auto context = mContext;

                mCommandQueue.Submit([context, verticesCount, baseVertex, instancesCount](){
                    context->Draw(verticesCount, baseVertex, instancesCount);
                });
            }

            void DrawIndexed(uint32 indexCount, uint32 baseIndex, uint32 instanceCount) {
                assert(mBeginCalled);
                assert(mRenderPass);

                auto context = mContext;

                mCommandQueue.Submit([context, indexCount, baseIndex, instanceCount](){
                    context->DrawIndexed(indexCount, 0, baseIndex, instanceCount);
                });
            }

            void EndRenderPass() {
                assert(mBeginCalled);
                assert(mRenderPass);

                mRenderPass = false;
                auto context = mContext;

                mCommandQueue.Submit([context](){
                    context->EndRenderPass();
                });
            }

            /**
             * Finish scene rendering.
             * @note Call Flush() to send commands for rendering.
             */
            void EndScene() {
                assert(mBeginCalled);

                mBeginCalled = false;
                auto context = mContext;

                mCommandQueue.Submit([context](){
                    context->EndScene();
                });
            }

            /**
             * Flush commands for rendering.
             *
             * This command commits captured rendering and update commands for
             * the execution to the RHI thread. This commands list will be placed into
             * queue, and the later fetched by RHI thread and executed on its side.
             * Actual execution will happen as soon as RHI thread reaches this
             * commands list. It might happen in this or in the next logical frame processing.
             */
            void Flush() {
                assert(!mBeginCalled);
                mCommandQueue.Commit();
            }

        protected:
            AsyncCommandQueue<> mCommandQueue;
            Context* mContext;
            bool mBeginCalled = false;
            bool mRenderPass = false;
        };

    }
}

#endif //BERSERK_RHICMDLIST_HPP