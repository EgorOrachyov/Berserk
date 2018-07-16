//
// Created by Egor Orachyov on 16.07.2018.
//

#ifndef BERSERKENGINE_GLFRAMEBUFFEROBJECT_H
#define BERSERKENGINE_GLFRAMEBUFFEROBJECT_H

#include "Essential/Types.h"

namespace Berserk
{

    class GLFrameBufferObject
    {
    public:

        GLFrameBufferObject();
        ~GLFrameBufferObject();

        void init();
        void destroy();

    private:

        UINT32 mFrameBuffer;
        UINT32 mRenderBuffer;
        UINT16 mWidth;
        UINT16 mHeight;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLFRAMEBUFFEROBJECT_H