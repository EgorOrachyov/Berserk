//
// Created by Egor Orachyov on 28.07.2018.
//

#ifndef BERSERKENGINE_STATICMESHPNT_H
#define BERSERKENGINE_STATICMESHPNT_H

#include "Objects/GPU/GPUBuffer.h"
#include "Containers/ArrayList.h"
#include "Components/VertexComponents.h"

namespace Berserk
{

    class StaticMeshPNT
    {
    public:

        StaticMeshPNT(const CStaticString &name, ArrayList<VertexPNT> &vertices, ArrayList<UINT16> &indices);
        GPUBuffer *getGPUBuffer();

    private:

        GPUBuffer *mGPUBuffer;

    };

} // namespace Berserk

#endif //BERSERKENGINE_STATICMESHPNT_H