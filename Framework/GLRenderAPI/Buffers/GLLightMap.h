//
// Created by Egor Orachyov on 26.08.2018.
//

#ifndef BERSERKENGINE_GLLIGHTMAP_H
#define BERSERKENGINE_GLLIGHTMAP_H

#include "Render/LightMap.h"

namespace Berserk
{

    class GLLightMap : public LightMap
    {
    public:

        virtual ~GLLightMap() = default;

        void create(UINT32 width, UINT32 height) override;
        void destroy() override;
        void useAsFBO() override;
        void useAsUniform(UINT32 binding) override;

    private:

        UINT32 mFBOHandle;
        UINT32 mTextureHandle;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLLIGHTMAP_H