/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Resources/Mesh.h>
#include <Reflection/ClassBuilder.h>

namespace Berserk {

    Mesh::Mesh(EPrimitivesType primitivesType, MeshFormat format, const Aabbf& aabb, TArray<MeshNode> nodes, uint32 verticesCount, BinaryData vertexData) {
        mPrimitivesType = primitivesType;
        mFormat = format;
        mAabb = aabb;
        mNodes = std::move(nodes);
        mVerticesCount = verticesCount;
        mVertexData = std::move(vertexData);
    }
    
    Mesh::Mesh(EPrimitivesType primitivesType, MeshFormat format, const Aabbf& aabb, TArray<MeshNode> nodes, uint32 verticesCount, uint32 indicesCount, BinaryData vertexData, BinaryData indexData, EIndexType type) {
        mPrimitivesType = primitivesType;
        mFormat = format;
        mAabb = aabb;
        mNodes = std::move(nodes);
        mVerticesCount = verticesCount;
        mIndicesCount = indicesCount;
        mVertexData = std::move(vertexData);
        mIndexData = std::move(indexData);
        mIndexType = type;
        mIsIndexed = true;
    }

    void Mesh::registerInfo() {
        ClassBuilder<Mesh> builder;

        builder
            .registerClass();
    }

    MeshImportOptions& Mesh::getDefaultImportOptions() {
        static MeshImportOptions options;
        return options;
    }

}