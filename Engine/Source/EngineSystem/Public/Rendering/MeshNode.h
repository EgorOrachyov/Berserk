//
// Created by Egor Orachyov on 2019-08-07.
//

#ifndef BERSERK_MESHNODE_H
#define BERSERK_MESHNODE_H

#include <Containers/TArray.h>
#include <Rendering/Definitions.h>
#include <Object/Allocatable.h>

namespace Berserk
{

    /** Basic mesh buffer with vertices data and single <optional> material index */
    class ENGINE_API MeshNode final : public Allocatable
    {
    public:

        /** Move mesh node data from somewhere */
        MeshNode(uint32 materialIndex,
                 uint32 verticesCount, uint32 verticesOffset,
                 uint32 indicesCount, uint32 indicesOffset)
            : mMaterialIndex(materialIndex),
              mVerticesCount(verticesCount),
              mVerticesOffset(verticesOffset),
              mIndicesCount(indicesCount),
              mIndicesOffset(indicesOffset)
        {

        }

        /** Move mesh node data from somewhere [ignore material] */
        MeshNode(uint32 verticesCount, uint32 verticesOffset,
                 uint32 indicesCount, uint32 indicesOffset)
            : mMaterialIndex(0xffffffff),
              mVerticesCount(verticesCount),
              mVerticesOffset(verticesOffset),
              mIndicesCount(indicesCount),
              mIndicesOffset(indicesOffset)
        {

        }

        /** Set index of the material in the mesh */
        void setMaterialIndex(uint32 index) { mMaterialIndex = index; }

        /** @return Index of the material in the mesh */
        uint32 getMaterialIndex() const { return mMaterialIndex; }

        /** @return Vertices count in the buffer */
        uint32 getVerticesCount() const { return mVerticesCount; }

        /** @return Vertices offset in the buffer [in bytes] */
        uint32 getVerticesOffset() const { return mVerticesOffset; }

        /** @return Indices count in the buffer */
        uint32 getIndicesCount() const { return mIndicesCount; }

        /** @return Indices offset in the buffer [in bytes] */
        uint32 getIndicesOffset() const { return mIndicesOffset; }

    protected:

        uint32 mMaterialIndex;
        uint32 mVerticesCount;
        uint32 mVerticesOffset;
        uint32 mIndicesCount;
        uint32 mIndicesOffset;

    };

} // namespace Berserk

#endif //BERSERK_MESHNODE_H