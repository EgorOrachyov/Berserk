//
// Created by Egor Orachyov on 29.07.2018.
//

#ifndef BERSERKENGINE_STATICMESH_H
#define BERSERKENGINE_STATICMESH_H

#include "Objects/GPU/GPUBuffer.h"
#include "Containers/ArrayList.h"
#include "Components/VertexComponents.h"

namespace Berserk
{

    class StaticMesh
    {
    public:

        StaticMesh(const CStaticString &name, ArrayList<Vertex> &vertices, ArrayList<UINT16> &indices);
        GPUBuffer *getGPUBuffer();

    private:

        GPUBuffer *mGPUBuffer;

    };

} // namespace Berserk

#endif //BERSERKENGINE_STATICMESH_H