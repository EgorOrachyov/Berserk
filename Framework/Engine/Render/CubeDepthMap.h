//
// Created by Egor Orachyov on 12.08.2018.
//

#ifndef BERSERKENGINE_CUBEDEPTHMAP_H
#define BERSERKENGINE_CUBEDEPTHMAP_H

#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"


namespace Berserk
{

    class GRAPHICS_API CubeDepthMap
    {
    public:

        virtual void create(UINT32 size, UINT32 textureSlot) = 0;
        virtual void destroy() = 0;

        virtual void useAsFBO() = 0;
        virtual void useAsUniform() = 0;

        virtual UINT32 getSize() const = 0;
    };

} // namespace Berserk

#endif //BERSERKENGINE_CUBEDEPTHMAP_H