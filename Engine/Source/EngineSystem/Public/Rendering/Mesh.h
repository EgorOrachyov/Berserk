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

        Mesh(EBufferUsage vertexBufferUsage,
             EIndexType indicesType, EDataLayout verticesType, EPrimitiveType primitiveType,
             TArray<MeshNode> &meshNodes, TArray<uint8> &verticesBuffer, TArray<uint8> &indicesBuffer)
             : mVertexBufferUsage(vertexBufferUsage),
               mIndicesType(indicesType),
               mVerticesType(verticesType),
               mPrimitiveType(primitiveType),
               mMeshNodes(std::move(meshNodes)),
               mVerticesBuffer(std::move(verticesBuffer)),
               mIndicesBuffer(std::move(indicesBuffer))
        {

        }

        Mesh(const Mesh& mesh) = default;

        Mesh(Mesh&& mesh) noexcept = default;

        ~Mesh() override = default;

        /**
         * Set name of the resource
         * @warning Only for resource manager
         */
        void setName(const String& name) { mResourceName = name; }

        /**
         * Set uuid of the resource
         * @warning Only for resource manager
         */
        void setUUID(const UUID& uuid) { mResourceUUID = uuid; }

        /** @return Vertex buffer usage */
        EBufferUsage getVertexBufferUsage() const { return mVertexBufferUsage; }

        /** @return Vertex buffer usage */
        EBufferUsage getIndexBufferUsage() const { return BU_StaticDraw; }

        /** @return Type of the mesh indices */
        EIndexType getIndicesType() const { return mIndicesType; }

        /** @return Type of the mesh vertices */
        EDataLayout getVerticesType() const { return mVerticesType; }

        /** @return Type of the mesh primitives */
        EPrimitiveType getPrimitiveType() const { return mPrimitiveType; }

        /** @return Nodes data of this mesh  */
        const TArray<MeshNode> &getMeshNodes() const { return mMeshNodes; }

        /** @return Raw vertices data buffer */
        const TArray<uint8> &getVerticesBuffer() const { return mVerticesBuffer; }

        /** @return Raw indices data buffer */
        const TArray<uint8> &getIndicesBuffer() const { return mIndicesBuffer; }

        /** @return Nodes data of this mesh  */
        TArray<MeshNode> &getMeshNodes() { return mMeshNodes; }

        /** @return Raw vertices data buffer */
        TArray<uint8> &getVerticesBuffer() { return mVerticesBuffer; }

        /** @return Raw indices data buffer */
        TArray<uint8> &getIndicesBuffer() { return mIndicesBuffer; }

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

        EBufferUsage mVertexBufferUsage;
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