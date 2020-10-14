/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIDRAWPRIMITIVE_H
#define BERSERK_RHIDRAWPRIMITIVE_H

#include <BerserkCore/RHI/RHIVertexBuffer.hpp>
#include <BerserkCore/RHI/RHIIndexBuffer.hpp>
#include <BerserkCore/TPtrShared.hpp>

namespace Berserk {

    using RHIVertexBuffers = TArrayFixed<TPtrShared<VertexBuffer>,RHI::MAX_VERTEX_BUFFER_COUNT>;

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