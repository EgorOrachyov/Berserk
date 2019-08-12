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
     * Stores geometry in format of vertices of defined type with
     * indices composed as primitives of special draw type.
     *
     * @note Allows to store material indices inside mesh nodes,
     *       however has no internal mechanism to support material storage.
     *       It means that mesh is raw geometry. All the other properties
     *       must be defined outside.
     *
     * @note Mesh nodes, which represents single part of the mesh with
     *       optional material and transformation indices are
     *       stored only internally by data and could not exits without mesh.
     *
     * @note Mesh serialization in special engine .mesh format
     *       will be added later.
     */
    class ENGINE_API Mesh final : public Resource
    {
    public:

        Mesh(EBufferUsage vertexBufferUsage,
             EIndexType indicesType, EDataLayout verticesType, EPrimitiveType primitiveType,
             TArray<MeshNode> &meshNodes, TArray<uint8> &verticesBuffer, TArray<uint8> &indicesBuffer);

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

    protected:

        EBufferUsage mVertexBufferUsage;
        EIndexType mIndicesType;
        EDataLayout mVerticesType;
        EPrimitiveType mPrimitiveType;

        TArray<MeshNode> mMeshNodes;
        TArray<uint8> mVerticesBuffer;
        TArray<uint8> mIndicesBuffer;

    };

} // namespace Berserk

#endif //BERSERK_MESH_H