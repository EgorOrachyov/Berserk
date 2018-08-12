//
// Created by Egor Orachyov on 16.07.2018.
//

#ifndef BERSERKENGINE_CUBE_H
#define BERSERKENGINE_CUBE_H

#include "Render/RenderSystem.h"

namespace Berserk
{

    class Cube
    {
    public:
        static RenderMesh *create(UINT32 mask, FLOAT32 size, const CStaticString& name);
    };

} // namespace Berserk

#endif //BERSERKENGINE_CUBE_H