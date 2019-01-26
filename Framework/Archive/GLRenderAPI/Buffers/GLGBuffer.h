//
// Created by Egor Orachyov on 18.08.2018.
//

#ifndef BERSERKENGINE_GLGBUFFER_H
#define BERSERKENGINE_GLGBUFFER_H

#include "Render/GBuffer.h"

namespace Berserk
{

    class GLGBuffer : public GBuffer
    {
    private:

        struct Info
        {
        public:

            UINT32 mHandle;
            UINT32 mSlot;
        };

    public:

        virtual ~GLGBuffer() = default;

        void init(UINT32 width, UINT32 height) override;
        void destroy() override;

        void useAsFBO() override;
        void useAsUniform() override;
        void useAsUniformLayout(UINT32 index, UINT32 binding) override;
        void useAsUniformDepthBuffer(UINT32 binding) override;

        UINT32 getWidht() override;
        UINT32 getHeight() override;

    private:

        UINT32 mFBOHandle;
        UINT32 mDepthHandle;
        UINT32 mWidht;
        UINT32 mHeight;
        Info   mTexture[GBufferInfo::GBI_SUPPORTED_LAYOUTS];

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLGBUFFER_H