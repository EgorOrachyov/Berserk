/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VERTEXARRAY_H
#define BERSERK_VERTEXARRAY_H

#include <RHI/RHIVertexBuffer.h>
#include <RHI/RHIIndexBuffer.h>
#include <RHI/RHIArrayObject.h>
#include <RHI/RHIDrawList.h>
#include <RenderResources/RenderResource.h>
#include <RenderResources/VertexDeclaration.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Vertex Array
         *
         * Complete set of the vertex and index data with vertex layout,
         * required for the rendering on the GPU side.
         */
        class VertexArray : public RenderResource {
        public:

            VertexArray(class VertexArrayBuilder& builder);
            ~VertexArray() override = default;

            bool isInitialized() const override;
            bool isInitializedRHI() const override;
            CString getFriendlyName() const override;

            /** @return True if has index buffer and uses indices */
            bool hasIndices() const { return mIndexBuffer.isNotNull(); }

            /** @return True if has instanced drawing */
            bool hasInstances() const { return mInstancesCount > 1; }

            /** @return Type of used indices */
            EIndexType getIndicesType() const { return mIndicesType; }

            /** @return Number of vertices */
            uint32 getVerticesCount() const { return mVerticesCount; }

            /** @return Indices count (0 if does not use indices for rendering) */
            uint32 getIndicesCount() const { return mIndicesCount; }

            /** @return Number of instance (1 if draw only single instance) */
            uint32 getInstancesCount() const { return mInstancesCount; }

            /** @return Vertex buffers */
            const TArrayStatic<TPtrShared<RHIVertexBuffer>> &getVertexBuffers() const { return mVertexBuffers; }

            /** @return Index buffer */
            const TPtrShared<RHIIndexBuffer>& getIndexBuffer() const { return mIndexBuffer; }

            /** @return Vertex declaration */
            const TPtrShared<VertexDeclaration>& getDeclaration() const { return mDeclaration; }

            /** @return Array object RHI handle */
            const TPtrShared<RHIArrayObject>& getRHI() const { return mArrayObjectRHI; }

            /** Bind array object and draw in the draw list */
            void draw(RHIDrawList &drawList);

        private:

            /** Optional name (for debug) */
            CString mName;

            /** VertexBuffers with the actual data */
            TArrayStatic<TPtrShared<RHIVertexBuffer>> mVertexBuffers;

            /** Optional index buffer (if vertices indexed) */
            TPtrShared<RHIIndexBuffer> mIndexBuffer;

            /** Actual RHI resource of this vertex array */
            TPtrShared<RHIArrayObject> mArrayObjectRHI;

            /** Actual layout of the elements in the vertex buffers */
            TPtrShared<VertexDeclaration> mDeclaration;

            /** Type of the indices */
            EIndexType mIndicesType;

            /** Number of the vertices in the vertex buffer */
            uint32 mVerticesCount;

            /** Number of the indices in the index buffer */
            uint32 mIndicesCount;

            /** Number of the instances to render from this buffer */
            uint32 mInstancesCount;

        };

    }
}

#endif //BERSERK_VERTEXARRAY_H