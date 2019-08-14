//
// Created by Egor Orachyov on 2019-07-16.
//

#ifndef BERSERK_RENDERINGCOMMON_H
#define BERSERK_RENDERINGCOMMON_H

namespace Berserk
{

    struct Viewport
    {

        Viewport(uint32 x, uint32 y, uint32 width, uint32 height)
            : x(x), y(y), width(width), height(height)
        {}

        /** X point coordinate to start writing in screen (fbo) buffer */
        uint32 x = 0;

        /** Y point coordinate to start writing in screen (fbo) buffer */
        uint32 y = 0;

        /** Width offset for screen (fbo) writing */
        uint32 width = 0;

        /** Height offset for screen (fbo) writing */
        uint32 height = 0;

    };

} // namespace Berserk

#endif //BERSERK_RENDERINGCOMMON_H
