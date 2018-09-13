//
// Created by Egor Orachyov on 27.07.2018.
//

#ifndef BERSERKENGINE_RENDERCOMPONENT_H
#define BERSERKENGINE_RENDERCOMPONENT_H

#include "Math/Matrix4x4f.h"
#include "Render/GPUBuffer.h"

namespace Berserk
{

    class RenderComponent
    {
    public:

        Matrix4x4f mModel;
        GPUBuffer *mGPUBufer;
    };

}

#endif //BERSERKENGINE_RENDERCOMPONENT_H