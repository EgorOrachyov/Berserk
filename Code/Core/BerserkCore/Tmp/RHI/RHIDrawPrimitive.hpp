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

#ifndef BERSERK_RHIDRAWPRIMITIVE_H
#define BERSERK_RHIDRAWPRIMITIVE_H

#include <BerserkCore/RHI/RHIVertexBuffer.hpp>
#include <BerserkCore/RHI/RHIIndexBuffer.hpp>
#include <BerserkCore/TPtrShared.hpp>

namespace Berserk {

    using RHIVertexBuffers = TArrayFixed<TPtrShared<RHIVertexBuffer>,RHI::MAX_VERTEX_BUFFER_COUNT>;

    class RHIDrawPrimitive: public RHIResource {
    public:
        ~RHIDrawPrimitive() override = default;
        const RHIVertexBuffers& GetVertexBuffers() const { return mVertexBuffers; }
        const TPtrShared<RHIIndexBuffer>& GetIndexBuffer() const { return mIndexBuffer; }
        uint32 GetInstancesCount() const { return mInstancesCount; }
        uint32 GetVerticesToDraw() const { return IsIndexedDraw()? 0: mCount; }
        uint32 GetIndicesToDraw() const { return IsIndexedDraw()? mCount: 0; }
        uint32 GetVertexOffset() const { return mOffset; }
        bool IsIndexedDraw() const { return mIndexBuffer.IsNotNull(); }

    protected:
        RHIVertexBuffers mVertexBuffers;
        TPtrShared<RHIIndexBuffer> mIndexBuffer;
        uint32 mInstancesCount;
        uint32 mCount;
        uint32 mOffset;
        EPrimitivesType mPrimitivesType;
    };

}

#endif //BERSERK_RHIDRAWPRIMITIVE_H