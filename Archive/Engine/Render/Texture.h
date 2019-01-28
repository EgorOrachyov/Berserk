//
// Created by Egor Orachyov on 08.08.2018.
//

#ifndef BERSERKENGINE_TEXTURE_H
#define BERSERKENGINE_TEXTURE_H

#include "RenderResource.h"

namespace Berserk
{
    enum TextureType
    {
        TT_NO_TYPE,
        TT_NORMAL_MAP,
        TT_CUBE_MAP,
        TT_SPECULAR_MAP,
        TT_DIFFUSE_MAP
    };

    class Texture : public RenderResource
    {
    public:

        virtual ~Texture() = default;

        virtual void setType(TextureType type) = 0;
        virtual TextureType getType() const = 0;

        virtual UINT32 getWidth() const = 0;
        virtual UINT32 getHeight() const = 0;
        virtual void   getSize(UINT32& width, UINT32& height) const = 0;

        virtual void use(UINT32 textureSlot) const = 0;

        virtual void setWrapping(UINT32 s, UINT32 t) = 0;
        virtual void setFiltering(UINT32 min, UINT32 mag) = 0;

        virtual bool isMipMapsGenerated() const = 0;
        virtual bool isLoaded() const = 0;

    };
}

#endif //BERSERKENGINE_TEXTURE_H