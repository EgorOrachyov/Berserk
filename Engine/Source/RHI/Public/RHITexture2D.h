//
// Created by Egor Orachyov on 2019-07-11.
//

#ifndef BERSERK_RHITEXTURE2D_H
#define BERSERK_RHITEXTURE2D_H

#include <RHITexture.h>

namespace Berserk
{

    class GRAPHICS_API RHITexture2D : public RHITexture
    {
    public:

        virtual ~RHITexture2D() = default;

        /** @return Texture width (for 0-LOD) */
        virtual uint32 getWidth() const = 0;

        /** @return Texture height (for 0-LOD) */
        virtual uint32 getHeight() const = 0;

    };

    typedef TSharedPtr<RHITexture2D> RHITexture2DRef;

} // namespace Berserk

#endif //BERSERK_RHITEXTURE2D_H