//
// Created by Egor Orachyov on 10.08.2018.
//

#ifndef BERSERKENGINE_GLSCREENPLANE_H
#define BERSERKENGINE_GLSCREENPLANE_H

#include "Buffers/GLGPUBuffer.h"

namespace Berserk
{

    class GLScreenPlane
    {
    public:

        void init();
        void use();

    private:

        GLGPUBuffer mBuffer;
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLSCREENPLANE_H