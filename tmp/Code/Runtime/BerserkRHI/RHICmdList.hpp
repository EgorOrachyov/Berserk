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
#include <BerserkCore/Templates/SmartPointer.hpp>
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

            void BeginScene(const SharedPtr<Window>& window) {
                assert(!mBeginCalled);

                mBeginCalled = true;
                auto context = mContext;

                if (window)
                    window->AddUnsafeUsage();

                mCommandQueue.Submit([context, window](){
                    context->BeginScene(window);
                });
            }

            void BeginParallel() {
                assert(mBeginCalled);
                assert(!mRenderPass);

                auto context = mContext;

                mCommandQueue.Submit([context](){
                    context->BeginParallel();
                });
            }

            void EndParallel() {
                assert(mBeginCalled);
                assert(!mRenderPass);

                auto context = mContext;

                mCommandQueue.Submit([context](){
                    context->EndParallel();
                });
            }

            void BeginSequence() {
                assert(mBeginCalled);
                assert(!mRenderPass);

                auto context = mContext;

                mCommandQueue.Submit([context](){
                    context->BeginSequence();
                });
            }

            void EndSequence() {
                assert(mBeginCalled);
                assert(!mRenderPass);

                auto context = mContext;

                mCommandQueue.Submit([context](){
                    context->EndSequence();
                });
            }

            void UpdateVertexBuffer(const RcPtr<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RcPtr<Data> &memory) {
                assert(!mRenderPass);
                assert(buffer);
                assert(memory);

                auto context = mContext;

                mCommandQueue.Submit([context, buffer, byteOffset, byteSize, memory](){
                    context->UpdateVertexBuffer(buffer, byteOffset, byteSize, memory->GetData());
                });
            }

            void UpdateIndexBuffer(const RcPtr<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RcPtr<Data> &memory) {
                assert(!mRenderPass);
                assert(buffer);
                assert(memory);

                auto context = mContext;

                mCommandQueue.Submit([context, buffer, byteOffset, byteSize, memory](){
                    context->UpdateIndexBuffer(buffer, byteOffset, byteSize, memory->GetData());
                });
            }

            void UpdateUniformBuffer(const RcPtr<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RcPtr<Data> &memory) {
                assert(!mRenderPass);
                assert(buffer);
                assert(memory);

                auto context = mContext;

                mCommandQueue.Submit([context, buffer, byteOffset, byteSize, memory](){
                    context->UpdateUniformBuffer(buffer, byteOffset, byteSize, memory->GetData());
                });
            }

            void UpdateTexture2D(const RcPtr<Texture> &texture, uint32 mipLevel, const Math::Rect2u& region, const RcPtr<PixelData>& memory) {
                assert(!mRenderPass);
                assert(texture);
                assert(memory);

                auto context = mContext;

                mCommandQueue.Submit([context, texture, mipLevel, region, memory](){
                    context->UpdateTexture2D(texture, mipLevel, region, *memory);
                });
            }

            void UpdateTexture2DArray(const RcPtr<Texture> &texture, uint32 arrayIndex, uint32 mipLevel, const Math::Rect2u& region, const RcPtr<PixelData>& memory) {
                assert(!mRenderPass);
                assert(texture);
                assert(memory);

                auto context = mContext;

                mCommandQueue.Submit([context, texture, arrayIndex, mipLevel, region, memory](){
                    context->UpdateTexture2DArray(texture, arrayIndex, mipLevel, region, *memory);
                });
            }

            void UpdateTextureCube(const RcPtr<Texture> &texture, TextureCubemapFace face, uint32 mipLevel, const Math::Rect2u& region, const RcPtr<PixelData>& memory) {
                assert(!mRenderPass);
                assert(texture);
                assert(memory);

                auto context = mContext;

                mCommandQueue.Submit([context, texture, face, mipLevel, region, memory](){
                    context->UpdateTextureCube(texture, face, mipLevel, region, *memory);
                });
            }

            void GenerateMipMaps(const RcPtr<Texture> &texture) {
                assert(!mRenderPass);
                assert(texture);

                auto context = mContext;

                mCommandQueue.Submit([context, texture](){
                    context->GenerateMipMaps(texture);
                });
            }

            void BeginRenderPass(const RenderPass& renderPass, const RcPtr<Framebuffer>& renderTarget) {
                assert(mBeginCalled);
                assert(!mRenderPass);
                assert(renderTarget);

                mRenderPass = true;
                auto context = mContext;

                mCommandQueue.Submit([context, renderPass, renderTarget](){
                    context->BeginRenderPass(renderPass, renderTarget);
                });
            }

            void BeginRenderPass(const RenderPass& renderPass) {
                assert(mBeginCalled);
                assert(!mRenderPass);

                mRenderPass = true;
                auto context = mContext;

                mCommandQueue.Submit([context, renderPass](){
                    context->BeginRenderPass(renderPass);
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

            void BindVertexBuffers(const ArrayFixed<RcPtr<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) {
                assert(mBeginCalled);
                assert(mRenderPass);

                auto context = mContext;

                mCommandQueue.Submit([context, buffers](){
                    context->BindVertexBuffers(buffers);
                });
            }

            void BindIndexBuffer(const RcPtr<IndexBuffer> &buffer, IndexType indexType) {
                assert(mBeginCalled);
                assert(mRenderPass);
                assert(buffer);

                auto context = mContext;

                mCommandQueue.Submit([context, buffer, indexType](){
                    context->BindIndexBuffer(buffer, indexType);
                });
            }

            void BindUniformBuffer(const RcPtr<UniformBuffer>& buffer, uint32 index, uint32 byteOffset, uint32 byteSize) {
                assert(mBeginCalled);
                assert(mRenderPass);
                assert(buffer);

                auto context = mContext;

                mCommandQueue.Submit([context, buffer, index, byteOffset, byteSize](){
                    context->BindUniformBuffer(buffer, index, byteOffset, byteSize);
                });
            }

            void BindTexture(const RcPtr<Texture> &texture, uint32 location) {
                BindTexture(texture, location, 0);
            }

            void BindSampler(const RcPtr<Sampler> &sampler, uint32 location) {
                BindSampler(sampler, location, 0);
            }

            void BindTexture(const RcPtr<Texture> &texture, uint32 location, uint32 arrayIndex) {
                assert(mBeginCalled);
                assert(mRenderPass);
                assert(texture);

                auto context = mContext;

                mCommandQueue.Submit([context, texture, location, arrayIndex](){
                    context->BindTexture(texture, location, arrayIndex);
                });
            }

            void BindSampler(const RcPtr<Sampler> &sampler, uint32 location, uint32 arrayIndex) {
                assert(mBeginCalled);
                assert(mRenderPass);
                assert(sampler);

                auto context = mContext;

                mCommandQueue.Submit([context, sampler, location, arrayIndex](){
                    context->BindSampler(sampler, location, arrayIndex);
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