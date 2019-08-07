//
// Created by Egor Orachyov on 2019-08-07.
//

#include "Rendering/MeshFactory.h"
#include <Memory/IAllocator.h>
#include <Rendering/VertexTypes.h>

namespace Berserk
{

    MeshFactory::MeshFactory(EBufferUsage bufferUsage, EIndexType indicesType,
                             EDataLayout verticesType, EPrimitiveType primitiveType,
                             IAllocator& allocator)
        : mAllocator(allocator),
          mVertexBufferUsage(bufferUsage),
          mIndicesType(indicesType),
          mVerticesType(verticesType),
          mPrimitiveType(primitiveType),
          mMeshNodes(allocator),
          mVerticesBuffer(allocator),
          mIndicesBuffer(allocator)
    {
        switch (indicesType)
        {
            case IT_UnsignedByte:
                mIndexSize = sizeof(uint8);
                break;
            case IT_UnsignedShort:
                mIndexSize = sizeof(uint16);
                break;
            case IT_UnsignedInt:
                mIndexSize = sizeof(uint32);
                break;
        }

        switch (verticesType)
        {
            case DL_Vertex:
                mVertexSize = sizeof(Vertf);
                break;
            case DL_VertexPN:
                mVertexSize = sizeof(VertPNf);
                break;
            case DL_VertexPT:
                mVertexSize = sizeof(VertPTf);
                break;
            case DL_VertexPNT:
                mVertexSize = sizeof(VertPNTf);
                break;
            case DL_VertexPNTBT:
                mVertexSize = sizeof(VertPNTBTf);
                break;
        }
    }

    void MeshFactory::addMeshNode(uint8* vertices, uint32 verticesCount,
                                  uint8* indices, uint32 indicesCount, uint32 materialIndex)
    {

        uint32 verticesOffset = mVerticesBuffer.getSize();
        uint32 indicesOffset = mIndicesBuffer.getSize();

        mMeshNodes.emplace(materialIndex, verticesCount, verticesOffset, indicesCount, indicesOffset);
        mVerticesBuffer.append(vertices, verticesCount * mVertexSize);
        mIndicesBuffer.append(indices, indicesCount * mIndexSize);
    }

    TSharedPtr<Mesh> MeshFactory::createMesh()
    {
        auto mesh = mAllocator.engnie_new<Mesh>(
                mVertexBufferUsage,
                mIndicesType,
                mVerticesType,
                mPrimitiveType,
                mMeshNodes,
                mVerticesBuffer,
                mIndicesBuffer);

        return TSharedPtr<Mesh>(mesh, &mAllocator);
    }

} // namespace Berserk
