//
// Created by Egor Orachyov on 12.08.2018.
//

#ifndef BERSERKENGINE_DEPTHMAP_H
#define BERSERKENGINE_DEPTHMAP_H

#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"

namespace Berserk
{

    class GRAPHICS_API DepthMap
    {
    public:

        virtual void create(UINT32 width, UINT32 height, UINT32 textureSlot) = 0;
        virtual void destroy() = 0;

        virtual void useAsFBO() = 0;
        virtual void useAsUniform() = 0;

        virtual UINT32 getWidth() const = 0;
        virtual UINT32 getHeight() const = 0;

    };

} // namespace Berserk

#endif //BERSERKENGINE_DEPTHMAP_H