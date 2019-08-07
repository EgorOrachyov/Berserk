//
// Created by Egor Orachyov on 2019-07-11.
//

#ifndef BERSERK_RHITEXTURECUBE_H
#define BERSERK_RHITEXTURECUBE_H

#include <RHI/RHITexture.h>

namespace Berserk
{

    class GRAPHICS_API RHITextureCube : public RHITexture
    {
    public:

        ~RHITextureCube() override = default;

        /** @return Texture size (as cube edge size) */
        virtual uint32 getSize() = 0;

    };

    typedef TSharedPtr<RHITextureCube> RHITextureCubeRef;

} // namespace Berserk

#endif //BERSERK_RHITEXTURECUBE_H