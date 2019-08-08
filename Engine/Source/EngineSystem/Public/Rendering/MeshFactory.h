//
// Created by Egor Orachyov on 2019-08-07.
//

#ifndef BERSERK_MESHFACTORY_H
#define BERSERK_MESHFACTORY_H

#include <Rendering/Mesh.h>

namespace Berserk
{

    /**
     * Allows to create mesh of specified from raw data and then emit this mesh.
     * @note Uses static draw buffer usage for indices
     */
    class ENGINE_API MeshFactory : public Allocatable
    {
    public:

        /**
         * Initialize factory to start create mesh of the specified type
         * @param bufferUsage Buffer usage for vertex buffer
         * @param indicesType Type of the indices in the index buffer of mesh
         * @param verticesType Type of the vertices in the mesh
         * @param primitiveType Type of the primitives, which describes surface mesh
         * @param allocator Memory allocator to allocate the mesh and its data
         */
        MeshFactory(EBufferUsage bufferUsage, EIndexType indicesType,
                    EDataLayout verticesType, EPrimitiveType primitiveType,
                    IAllocator& allocator = Allocator::get());

        /**
         * Adds mesh node to the mesh with specified data
         * @param vertices Buffer with vertices data
         * @param verticesSize Count of the vertices
         * @param indices Buffer with indices data
         * @param indicesCount Count of the indices
         * @param materialIndex Optional material index of the mesh node
         * @param transformIndex Optional transformation index of the mesh node
         */
        void addMeshNode(uint8* vertices, uint32 verticesCount,
                         uint8* indices, uint32 indicesCount,
                         uint32 materialIndex = MeshNode::NOT_USED_INDEX,
                         uint32 transformIndex = MeshNode::NOT_USED_INDEX);

        /**
         * Creates mesh from specified data to the factory
         * @note Factory becomes empty after mesh emitted
         * @return Created mesh node
         */
        TSharedPtr<Mesh> createMesh();

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

    protected:

        IAllocator& mAllocator;

        uint32 mIndexSize;
        uint32 mVertexSize;

        EBufferUsage mVertexBufferUsage;
        EIndexType mIndicesType;
        EDataLayout mVerticesType;
        EPrimitiveType mPrimitiveType;

        TArray<MeshNode> mMeshNodes;
        TArray<uint8> mVerticesBuffer;
        TArray<uint8> mIndicesBuffer;


    };

} // namespace Berserk

#endif //BERSERK_MESHFACTORY_H