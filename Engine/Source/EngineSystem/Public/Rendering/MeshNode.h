//
// Created by Egor Orachyov on 2019-08-07.
//

#ifndef BERSERK_MESHNODE_H
#define BERSERK_MESHNODE_H

#include <Containers/TArray.h>
#include <Rendering/Definitions.h>
#include <Misc/Allocatable.h>

namespace Berserk
{

    /** Basic mesh buffer with vertices data and single <optional> material index */
    class ENGINE_API MeshNode final : public Allocatable
    {
    public:

        /** Move mesh node data from somewhere */
        MeshNode(uint32 materialIndex, uint32 transformIndex,
                 uint32 verticesCount, uint32 verticesOffset,
                 uint32 indicesCount, uint32 indicesOffset)

            : mMaterialIndex(materialIndex),
              mTransformIndex(transformIndex),
              mVerticesCount(verticesCount),
              mVerticesOffset(verticesOffset),
              mIndicesCount(indicesCount),
              mIndicesOffset(indicesOffset)
        {

        }

        /** Move mesh node data from somewhere [ignore material] */
        MeshNode(uint32 verticesCount, uint32 verticesOffset,
                 uint32 indicesCount, uint32 indicesOffset)

            : mMaterialIndex(NOT_USED_INDEX),
              mTransformIndex(NOT_USED_INDEX),
              mVerticesCount(verticesCount),
              mVerticesOffset(verticesOffset),
              mIndicesCount(indicesCount),
              mIndicesOffset(indicesOffset)
        {

        }

        /** Set index of the material in the mesh */
        void setMaterialIndex(uint32 index) { mMaterialIndex = index; }

        /** Set index of the transformation in the mesh */
        void setTransformIndex(uint32 index) { mTransformIndex = index; }

        /** @return True if this mesh node has material */
        bool isMaterialPresent() const { return mMaterialIndex != NOT_USED_INDEX; }

        /** @return True if this mesh node has transformation */
        bool isTransformPresent() const { return mMaterialIndex != NOT_USED_INDEX; }

        /** @return Index of the material in the mesh */
        uint32 getMaterialIndex() const { return mMaterialIndex; }

        /** @return Index of the transformation in the mesh */
        uint32 getTransformIndex() const { return mTransformIndex; }

        /** @return Vertices count in the buffer */
        uint32 getVerticesCount() const { return mVerticesCount; }

        /** @return Vertices offset in the buffer [in bytes] */
        uint32 getVerticesOffset() const { return mVerticesOffset; }

        /** @return Indices count in the buffer */
        uint32 getIndicesCount() const { return mIndicesCount; }

        /** @return Indices offset in the buffer [in bytes] */
        uint32 getIndicesOffset() const { return mIndicesOffset; }

    public:

        /** Shows, whether index not used */
        static const uint32 NOT_USED_INDEX = 0xffffffff;

    protected:

        uint32 mMaterialIndex;
        uint32 mTransformIndex;
        uint32 mVerticesCount;
        uint32 mVerticesOffset;
        uint32 mIndicesCount;
        uint32 mIndicesOffset;

    };

} // namespace Berserk

#endif //BERSERK_MESHNODE_H