/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VERTEXARRAYBUILDER_H
#define BERSERK_VERTEXARRAYBUILDER_H

#include <RHI/RHIVertexBuffer.h>
#include <RHI/RHIIndexBuffer.h>
#include <RHI/RHIArrayObject.h>
#include <RenderResources/RenderResource.h>
#include <RenderResources/VertexDeclaration.h>

namespace Berserk {
    namespace Render {

        /** Utility to build vertex array and provide to it data */
        class VertexArrayBuilder {
        public:

            using Builder = VertexArrayBuilder;

            Builder& setName(const CString& name);

            Builder& setVertexDeclaration(const TPtrShared<VertexDeclaration>& declaration);

            Builder& addIndexBuffer(uint32 indicesCount, EIndexType type, const TPtrShared<RHIIndexBuffer> &buffer);

            Builder& addIndexBuffer(uint32 indicesCount, EIndexType type, EMemoryType memoryType = EMemoryType::Static);

            Builder& setVerticesCount(uint32 verticesCount);

            Builder& addVertexBuffer(const CString& bufferName, const TPtrShared<RHIVertexBuffer> &buffer);

            Builder& addVertexBuffer(const CString& bufferName, EMemoryType memoryType = EMemoryType::Static);

            Builder& setInstancesCount(uint32 instancesCount);

            Builder& setIndicesData(const uint32* data);

            Builder& setVertexBufferData(const CString& bufferName, const void* data);

            const TPtrShared<class VertexArray> &buildShared();

        private:

            friend class VertexArray;

            CString mName;
            TArrayStatic<TPtrShared<RHIVertexBuffer>> mVertexBuffers;
            TPtrShared<RHIIndexBuffer> mIndexBuffer;
            TPtrShared<RHIArrayObject> mArrayObjectRHI;
            TPtrShared<VertexDeclaration> mDeclaration;
            EIndexType mIndicesType = EIndexType::Uint32;
            uint32 mVerticesCount = 0;
            uint32 mIndicesCount = 0;
            uint32 mInstancesCount = 1;
            TPtrShared<class VertexArray> mInstance;
        };

    }
}

#endif //BERSERK_VERTEXARRAYBUILDER_H
