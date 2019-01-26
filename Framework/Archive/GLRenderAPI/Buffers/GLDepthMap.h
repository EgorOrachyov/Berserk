//
// Created by Egor Orachyov on 12.08.2018.
//

#ifndef BERSERKENGINE_GLDEPTHMAP_H
#define BERSERKENGINE_GLDEPTHMAP_H

#include "Render/DepthMap.h"

namespace Berserk
{

    class GLDepthMap : public DepthMap
    {
    public:

        void create(UINT32 width, UINT32 height, UINT32 textureSlot)  override;
        void destroy()  override;

        void useAsFBO()  override;
        void useAsUniform()  override;

        UINT32 getWidth() const override;
        UINT32 getHeight() const override;

    private:

        UINT32 mWidth;
        UINT32 mHeight;
        UINT32 mTextureSlot;
        UINT32 mFBOHandle;
        UINT32 mShadowMap;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLDEPTHMAP_H