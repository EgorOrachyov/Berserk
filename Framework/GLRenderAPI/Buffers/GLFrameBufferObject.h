//
// Created by Egor Orachyov on 16.07.2018.
//

#ifndef BERSERKENGINE_GLFRAMEBUFFEROBJECT_H
#define BERSERKENGINE_GLFRAMEBUFFEROBJECT_H

#include "Texture/GLTexture.h"
#include "Texture/GLSampler.h"
#include "Essential/Types.h"

namespace Berserk
{

    class GLFrameBufferObject
    {
    public:

        GLFrameBufferObject();
        ~GLFrameBufferObject();

        void init(UINT16 width, UINT16 height);
        void destroy();

        void use() const; // todo: maybe add more texture attachments (different render layouts)
        void disable() const;

        UINT32 getFBO() const;
        UINT32 getRBO() const;
        UINT32 getWidth() const;
        UINT32 getHeight() const;

    private:

        UINT32 mFBOHandle;
        UINT32 mRBOHandle;
        UINT32 mWidth;
        UINT32 mHeight;
        GLTexture mTexture;
        GLSampler mSampler;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLFRAMEBUFFEROBJECT_H