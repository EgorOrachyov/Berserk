//
// Created by Egor Orachyov on 2019-08-07.
//

#ifndef BERSERK_MESH_H
#define BERSERK_MESH_H

#include <Rendering/MeshNode.h>
#include <Rendering/Material.h>
#include <Resource/ResourceHandle.h>

namespace Berserk
{

    /**
     * Mesh indices and vertices data. Represents number of raw mesh nodes with
     * optional material indices. Stores type of the primitives, vertices and indices to pass to renderer.
     */
    class ENGINE_API Mesh final : public IResource, public Allocatable
    {
    public:

        Mesh(EIndexType indicesType, EDataLayout verticesType, EPrimitiveType primitiveType,
             TArray<MeshNode> meshNodes, TArray<uint8> verticesBuffer, TArray<uint8> indicesBuffer)
             : mIndicesType(indicesType),
               mVerticesType(verticesType),
               mPrimitiveType(primitiveType),
               mMeshNodes(std::move(meshNodes)),
               mVerticesBuffer(std::move(verticesBuffer)),
               mIndicesBuffer(std::move(indicesBuffer))
        {

        }

        EIndexType getIndicesType() const { return mIndicesType; }

        EDataLayout getVerticesType() const { return mVerticesType; }

        EPrimitiveType getPrimitiveType() const { return mPrimitiveType; }

        const TArray<MeshNode> &getNodes() const { return mMeshNodes; }

        const TArray<uint8> &getVerticesBuffer() const { return mVerticesBuffer; }

        const TArray<uint8> &getIndicesBuffer() const { return mIndicesBuffer; }

        uint32 getMemoryUsage() const override
        {
            return sizeof(Mesh) +
                   mMeshNodes.getMemoryUsage() +
                   mVerticesBuffer.getMemoryUsage() +
                   mIndicesBuffer.getMemoryUsage();
        }

        const String &getName() const override { return mResourceName; }

        const UUID &getUUID() const override { return mResourceUUID; }

    protected:

        EIndexType mIndicesType;
        EDataLayout mVerticesType;
        EPrimitiveType mPrimitiveType;

        TArray<MeshNode> mMeshNodes;
        TArray<uint8> mVerticesBuffer;
        TArray<uint8> mIndicesBuffer;

        String mResourceName;
        String mResourceUUID;

    };

} // namespace Berserk

#endif //BERSERK_MESH_H