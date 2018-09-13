//
// Created by Egor Orachyov on 26.08.2018.
//

#ifndef BERSERKENGINE_LIGHTMAP_H
#define BERSERKENGINE_LIGHTMAP_H

#include "Essential/Types.h"

namespace Berserk
{

    class LightMap
    {
    public:

        virtual ~LightMap() = default;

        virtual void create(UINT32 width, UINT32 height) = 0;
        virtual void destroy() = 0;
        virtual void useAsFBO() = 0;
        virtual void useAsUniform(UINT32 binding) = 0;
    };

} // namespace Berserk

#endif //BERSERKENGINE_LIGHTMAP_H
