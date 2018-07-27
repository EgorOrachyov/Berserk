//
// Created by Egor Orachyov on 28.07.2018.
//

#ifndef BERSERKENGINE_STATICMESHCOMPONENT_H
#define BERSERKENGINE_STATICMESHCOMPONENT_H

#include "Containers/ArrayList.h"
#include "Components/VertexComponents.h"

namespace Berserk
{
    class StaticMeshComponent
    {
    public:

        ArrayList<VertexPNT> mVertices;
        ArrayList<UINT32> mIndices;

    };

} // namespace Berserk

#endif //BERSERKENGINE_STATICMESHCOMPONENT_H