//
// Created by Egor Orachyov on 16.07.2018.
//

#ifndef BERSERKENGINE_PLANE_H
#define BERSERKENGINE_PLANE_H

#include "Render/RenderSystem.h"

namespace Berserk
{

    class Plane
    {
    public:
        static RenderMesh* create(UINT32 mask, FLOAT32 size, const CStaticString& name);
    };

} // namespace Berserk

#endif //BERSERKENGINE_PLANE_H