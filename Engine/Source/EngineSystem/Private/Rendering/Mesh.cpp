//
// Created by Egor Orachyov on 2019-08-12.
//

#include <Rendering/Mesh.h>

namespace Berserk
{

    Mesh::Mesh(Berserk::EBufferUsage vertexBufferUsage, Berserk::EIndexType indicesType,
               Berserk::EDataLayout verticesType, Berserk::EPrimitiveType primitiveType,
               Berserk::TArray<Berserk::MeshNode> &meshNodes, Berserk::TArray<Berserk::uint8> &verticesBuffer,
               Berserk::TArray<Berserk::uint8> &indicesBuffer)
            : mVertexBufferUsage(vertexBufferUsage),
              mIndicesType(indicesType),
              mVerticesType(verticesType),
              mPrimitiveType(primitiveType),
              mMeshNodes(std::move(meshNodes)),
              mVerticesBuffer(std::move(verticesBuffer)),
              mIndicesBuffer(std::move(indicesBuffer))
    {
        mSizeCPU = sizeof(Mesh) +
                   mMeshNodes.getMemoryUsage() +
                   mVerticesBuffer.getMemoryUsage() +
                   mIndicesBuffer.getMemoryUsage();

        mSizeGPU = mVerticesBuffer.getMemoryUsage() +
                   mIndicesBuffer.getMemoryUsage();
    }

}

