//
// Created by Egor Orachyov on 16.07.2018.
//

#ifndef BERSERKENGINE_GLSIZETESTING_H
#define BERSERKENGINE_GLSIZETESTING_H

#include "Buffers/GLFrameBufferObject.h"
#include "Texture/GLSampler.h"
#include "Texture/GLSampler.h"

void GLClassSizeTesting()
{
    using namespace Berserk;

    printf("FBO size %li \n", sizeof(GLFrameBufferObject));
    printf("Sampler size %li \n", sizeof(GLSampler));
    printf("Texture size %li \n", sizeof(GLTexture));

}

#endif //BERSERKENGINE_GLSIZETESTING_H
