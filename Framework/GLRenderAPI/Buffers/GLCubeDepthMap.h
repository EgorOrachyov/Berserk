//
// Created by Egor Orachyov on 12.08.2018.
//

#ifndef BERSERKENGINE_GLCUBEDEPTHMAP_H
#define BERSERKENGINE_GLCUBEDEPTHMAP_H

#include "Render/CubeDepthMap.h"

namespace Berserk
{

    class GLCubeDepthMap : public CubeDepthMap
    {
    public:

        void create(UINT32 size, UINT32 textureSlot) override;
        void destroy() override;

        void useAsFBO() override;
        void useAsUniform() override;

        UINT32 getSize() const override;

    private:

        UINT32 mSize;
        UINT32 mTextureSlot;
        UINT32 mFBOHandle;
        UINT32 mCubeMap;
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLCUBEDEPTHMAP_H