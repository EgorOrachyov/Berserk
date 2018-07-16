//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERKENGINE_GLTEXTURE_H
#define BERSERKENGINE_GLTEXTURE_H

#include "Essential/Types.h"

#include "GLTextureFormat.h"
#include "Essential/GLDataType.h"

namespace Berserk
{

    class GLTexture
    {
    public:

        GLTexture();
        ~GLTexture();

        void create(UINT32 width, UINT32 height);
        void create(UINT32 width, UINT32 height, GLInternalTextureFormat format);
        void create(UINT32 width, UINT32 height, GLInternalTextureFormat target,
                    GLImageFormat source, GLDataType actualType, void* data);

        void destroy();
        void use();

        void bindSlot(UINT16 slot);
        void bindSampler(UINT16 sampler); // todo: add samplers array

        UINT32 getHandle() const;
        UINT32 getSlot() const;
        UINT32 getSampler() const;

    private:

        UINT32 mHandle;
        UINT16 mSlot;
        UINT16 mSampler;


    };

} // namespace Berserk

#endif //BERSERKENGINE_GLTEXTURE_H