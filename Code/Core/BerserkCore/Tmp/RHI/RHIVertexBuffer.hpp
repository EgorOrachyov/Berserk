/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
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