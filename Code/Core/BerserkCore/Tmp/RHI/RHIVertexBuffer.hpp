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

#ifndef BERSERK_RHIVERTEXBUFFER_H
#define BERSERK_RHIVERTEXBUFFER_H

#include <BerserkCore/RHI/RHIDefinitions.hpp>
#include <BerserkCore/RHI/RHIResource.hpp>
#include <BerserkCore/Templates/ArrayFixed.hpp>

namespace Berserk {

    using RHIVertexAttributes = TArrayFixed<RHIVertexAttribute,RHI::MAX_VERTEX_ATTRIBUTE_COUNT>;

    class RHIVertexBuffer: public RHIResource {
    public:
        ~RHIVertexBuffer() override = default;
        uint32 GetVertexCount() const { return mVertexCount; }
        uint32 GetStride() const { return mStride; }
        uint32 GetSizeBytes() const { return mVertexCount * mStride; }
        const RHIVertexAttributes& GetAttributes() const { return mAttributes; }
        EBufferUsage GetBufferUsage() const { return mBufferUsage; }

    protected:
        RHIVertexAttributes mAttributes;
        uint32 mVertexCount = 0;
        uint32 mStride = 0;
        EBufferUsage mBufferUsage = EBufferUsage::MarkerUnknown;
    };

}

#endif //BERSERK_RHIVERTEXBUFFER_H