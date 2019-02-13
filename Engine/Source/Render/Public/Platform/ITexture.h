//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_ITEXTURE_H
#define BERSERK_ITEXTURE_H

#include "Public/Misc/Types.h"

namespace Berserk
{

    class ITexture
    {
    public:

        enum PixelFormat
        {
            FORMAT_R,
            FORMAT_RG,
            FORMAT_RGB,
            FORMAT_RGBA,
            FORMAT_DEPTH,
            FORMAT_DEPTH_AND_STENCIL,
        };

    public:

        virtual uint32 getWidth() = 0;

        virtual uint32 getHeight() = 0;

    };

} // namespace Berserk

#endif //BERSERK_ITEXTURE_H