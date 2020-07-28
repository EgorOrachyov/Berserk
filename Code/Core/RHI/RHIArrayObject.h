/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIARRAYOBJECT_H
#define BERSERK_RHIARRAYOBJECT_H

#include <RHI/RHIResource.h>
#include <RHI/RHIIndexBuffer.h>
#include <RHI/RHIVertexBuffer.h>
#include <RHI/RHIVertexDeclaration.h>

namespace Berserk {

    class RHIArrayObject : public RHIResource {
    public:
        ~RHIArrayObject() override = default;

        /** @return Vertex buffer attached to the vertex array object */
        const TArrayStatic<TPtrShared<RHIVertexBuffer>> &getVertexBuffers() const { return mVertexBuffers; };

        /** @return Optional index buffer of this array object */
        const TPtrShared<RHIIndexBuffer> &getIndexBuffer() const { return mIndexBuffer; };

        /** @return Vertex array declaration type */
        const TPtrShared<RHIVertexDeclaration> getVertexDeclaration() const { return mVertexDeclaration; }

        /** @return True, if uses index buffer */
        bool getUsesIndexBuffer() const { return mIndexBuffer != nullptr; }

        /** @return Type of the primitives in the array */
        EPrimitivesType getPrimitivesType() const { return mPrimitivesType; }

    protected:
        /** Vertex buffer attached to the vertex array object */
        TArrayStatic<TPtrShared<RHIVertexBuffer>> mVertexBuffers;

        /** Optional index buffer of this array object (indexing is primary technique for rendering) */
        TPtrShared<RHIIndexBuffer> mIndexBuffer;

        /** Vertex array declaration type */
        TPtrShared<RHIVertexDeclaration> mVertexDeclaration;

        /** Type of the primitives in the array */
        EPrimitivesType mPrimitivesType;
    };

}

#endif //BERSERK_RHIARRAYOBJECT_H
