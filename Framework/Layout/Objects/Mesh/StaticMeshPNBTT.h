//
// Created by Egor Orachyov on 29.07.2018.
//

#ifndef BERSERKENGINE_STATICMESHPNBTT_H
#define BERSERKENGINE_STATICMESHPNBTT_H

#include "Objects/GPU/GPUBuffer.h"
#include "Containers/ArrayList.h"
#include "Components/VertexComponents.h"

namespace Berserk
{

    class StaticMeshPNBTT
    {
    public:

        StaticMeshPNBTT(const CStaticString &name, ArrayList<VertexPNBTT> &vertices, ArrayList<UINT16> &indices);
        GPUBuffer *getGPUBuffer();

    private:

        GPUBuffer *mGPUBuffer;

    };

} // namespace Berserk

#endif //BERSERKENGINE_STATICMESHPNBTT_H