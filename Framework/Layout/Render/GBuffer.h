//
// Created by Egor Orachyov on 18.08.2018.
//

#ifndef BERSERKENGINE_GBUFFER_H
#define BERSERKENGINE_GBUFFER_H

#include "Essential/Types.h"

namespace Berserk
{

    enum GBufferInfo
    {
        GBI_POSITION_SLOT           = 0,
        GBI_NORMAL_SLOT             = 1,
        GBI_TEXTURE_COORDS_SLOT     = 2,
        GBI_DIFFUSE_COLOR_SLOT      = 3,
        GBI_SPECULAR_COLOR_SH_SLOT  = 4,

        GBI_SUPPORTED_LAYOUTS       = 4
    };

    class GBuffer
    {
    public:

        virtual ~GBuffer() = default;

        virtual void init(UINT32 width, UINT32 height) = 0;
        virtual void destroy() = 0;

        virtual void useAsFBO() = 0;
        virtual void useAsUniform() = 0;
        virtual void useAsUniformLayout(UINT32 index, UINT32 binding) = 0;

        virtual UINT32 getWidht() = 0;
        virtual UINT32 getHeight() = 0;
    };

} // namespace Berserk

#endif //BERSERKENGINE_GBUFFER_H
