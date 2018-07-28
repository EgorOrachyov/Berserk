//
// Created by Egor Orachyov on 29.07.2018.
//

#ifndef BERSERKENGINE_STATICMESHPN_H
#define BERSERKENGINE_STATICMESHPN_H

#include "Objects/GPU/GPUBuffer.h"
#include "Containers/ArrayList.h"
#include "Components/VertexComponents.h"

namespace Berserk
{

    class StaticMeshPN
    {
    public:

        StaticMeshPN(const CStaticString &name, ArrayList<VertexPN> &vertices, ArrayList<UINT16> &indices);
        GPUBuffer *getGPUBuffer();

    private:

        GPUBuffer *mGPUBuffer;

    };

} // namespace Berserk

#endif //BERSERKENGINE_STATICMESHPN_H