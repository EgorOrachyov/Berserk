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

#ifndef BERSERK_RHICMDENCODER_HPP
#define BERSERK_RHICMDENCODER_HPP

#include <BerserkCore/RHI/RHIVertexBuffer.hpp>
#include <BerserkCore/RHI/RHIIndexBuffer.hpp>
#include <BerserkCore/RHI/RHIDrawPrimitive.hpp>
#include <BerserkCore/RHI/RHIUniformBuffer.hpp>
#include <BerserkCore/RHI/RHIProgram.hpp>
#include <BerserkCore/RHI/RHIProgramCompilerEnv.hpp>
#include <BerserkCore/RHI/RHITexture.hpp>
#include <BerserkCore/RHI/RHIRenderTarget.hpp>
#include <BerserkCore/RHI/RHIMemoryBuffer.hpp>

namespace Berserk {

    class RHICmdEncoder {
    public:
        virtual ~RHICmdEncoder() = default;

        // Resource creation API
        virtual TPtrShared<RHIVertexBuffer> CreateVertexBuffer(const RHIVertexAttributes& attributes, uint32 vertexCount, EBufferUsage bufferUsage) = 0;
        virtual TPtrShared<RHIIndexBuffer> CreateIndexBuffer(EIndexType indexType, uint32 indexCount, EBufferUsage bufferUsage) = 0;
        virtual TPtrShared<RHIUniformBuffer> CreateUniformBuffer(uint32 size, EBufferUsage bufferUsage) = 0;
        virtual TPtrShared<RHIDrawPrimitive> CreateDrawPrimitive(EPrimitivesType primitivesType, uint32 instances, uint32 vertices, RHIVertexBuffers vertexBuffers) = 0;
        virtual TPtrShared<RHIDrawPrimitive> CreateDrawPrimitive(EPrimitivesType primitivesType, uint32 instances, uint32 indices, RHIVertexBuffers vertexBuffers, TPtrShared<RHIIndexBuffer> indexBuffer) = 0;
        virtual TPtrShared<RHIRenderTarget> CreateRenderTarget(RHIColorBuffers colorBuffers, TPtrShared<RHITexture> depthStencilBuffer) = 0;
        virtual TPtrShared<RHIProgram> CreateProgram(String name, EShaderLang language, RHIProgramStages stages, RHIProgramCompilerEnv compilerEnv, bool cacheSources) = 0;
        // todo: gpu textures (requires image class)
        virtual TPtrShared<RHITexture> CreateTexture2D() = 0;

        // Resource update API
        virtual void UpdateVertexBuffer(TPtrShared<RHIVertexBuffer> handle, uint32 offset, uint32 count, RHIMemoryBuffer memory) = 0;
        virtual void UpdateIndexBuffer(TPtrShared<RHIIndexBuffer> handle, uint32 offset, uint32 count, RHIMemoryBuffer memory) = 0;
        virtual void UpdateUniformBuffer(TPtrShared<RHIUniformBuffer> handle, uint32 offset, uint32 range, RHIMemoryBuffer memory) = 0;

        // Drawing API
        virtual void BeginRenderPass() = 0;
        virtual void BindPipeline() = 0;
        virtual void BindGpuProgram() = 0;
        virtual void BindUniformData() = 0;
        virtual void DrawPrimitive() = 0;

        virtual void Commit() = 0;
    };

}

#endif //BERSERK_RHICMDENCODER_HPP