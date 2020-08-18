/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MESH_H
#define BERSERK_MESH_H

#include <Resource.h>
#include <TEnumMask.h>
#include <RHI/RHIDefinitions.h>
#include <Containers/TArray.h>
#include <Containers/BinaryData.h>
#include <Resources/MeshNode.h>
#include <Resources/MeshAttribute.h>
#include <Resources/MeshImportOptions.h>

namespace Berserk {

    /**
     * @brief Mesh resource
     *
     * Represents packed mesh data, which consists of the packed vertex data
     * attributes in the single vertex buffer, optional index buffer,
     * and mesh nodes, which defines the subsections of the vertex and index data.
     *
     * Mesh nodes can be interpreted as sub-parts of this mesh and
     * can be renderer with different materials.
     */
    class Mesh : public Resource {
        BERSERK_CLASS(Mesh,Resource)

    public:

        /** Builds mesh without indices from data */
        Mesh(EPrimitivesType primitivesType, MeshFormat format, const Aabbf& aabb, TArray<MeshNode> nodes, uint32 verticesCount, BinaryData vertexData);

        /** Builds indexed mesh from data */
        Mesh(EPrimitivesType primitivesType, MeshFormat format, const Aabbf& aabb, TArray<MeshNode> nodes, uint32 verticesCount, uint32 indicesCount, BinaryData vertexData, BinaryData indexData, EIndexType type);

        ~Mesh() override = default;

        /** @return Packed vertex attributes data (per vertex with stride) */
        const BinaryData& getVertexData() const { return mVertexData; }

        /** @return Indices data */
        const BinaryData& getIndexData() const { return mIndexData; }

        /** @return Nodes of the mesh */
        const TArray<MeshNode>& getNodes() const { return mNodes; }

        /** @return Bounding volume of this mesh */
        const Aabbf& getBounds() const { return mAabb; }

        /** @return How vertices are arranged */
        EPrimitivesType getPrimitivesType() const { return mPrimitivesType; }

        /** @return Mesh format */
        MeshFormat getFormat() const { return mFormat; }

        /** @return Type of the mesh indices */
        EIndexType getIndexType() const { return mIndexType; }

        /** @return Number of the vertices in the mesh */
        uint32 getVerticesCount() const { return mVerticesCount; }

        /** @return Number of the indices in the mesh */
        uint32 getIndicesCount() const { return mIndicesCount; }

        /** @return Vertex attributes stride from format */
        uint32 getStride() const { return MeshFormatUtil::getStrideSizeForFormat(mFormat); }

        /** @return True if mesh uses indices */
        bool isIndexed() const { return mIsIndexed; }

        /** Class info */
        static void registerInfo();

        /** @return Default importing options */
        static MeshImportOptions& getDefaultImportOptions();

    private:

        /** Packed vertex attributes data (per vertex with stride) */
        BinaryData mVertexData;

        /** Indices data */
        BinaryData mIndexData;

        /** Actual nodes of the mesh */
        TArray<MeshNode> mNodes;

        /** Local space mesh bounds */
        Aabbf mAabb;

        /** How vertices are arranged */
        EPrimitivesType mPrimitivesType;

        /** Type of the mesh indices */
        EIndexType mIndexType = EIndexType::Unknown;

        /** Mesh vertex data format */
        MeshFormat mFormat;

        /** Number of the vertices in the mesh */
        uint32 mVerticesCount = 0;

        /** Number of the indices in the mesh */
        uint32 mIndicesCount = 0;

        /** This mesh uses indices */
        bool mIsIndexed = false;

    };

}

#endif //BERSERK_MESH_H