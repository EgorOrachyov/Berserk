//
// Created by Egor Orachyov on 2019-08-09.
//

#ifndef BERSERK_TEXTURE_H
#define BERSERK_TEXTURE_H

#include <Rendering/Definitions.h>
#include <Resource/Resource.h>
#include <Object/Allocatable.h>

namespace Berserk
{

    /** Types of the textures for RHI */
    enum ETextureType : uint8
    {
        TT_Texture1D,
        TT_Texture2D,
        TT_Texture3D,
        TT_TextureCube
    };

    /**
     * Base class for any texture, which could be used with RHI driver.
     *
     * @note Each texture internally handles RHI handler for hardware
     *       resource. Internal resource must be loaded at once via RHI driver.
     *
     * @note Texture must be fully loaded before be submitted for rendering
     *       to render system as part of the material.
     *
     * @note All the textures must be created by texture manager, which
     *       manages main thread and render thread resources. Implementation
     *       of the texture manager must be provided by rendering system.
     */
    class ENGINE_API Texture : public Resource
    {
    public:

        ~Texture() override = default;

        /** @return Texture width */
        uint32 getWidth() const { return mWidth; }

        /** @return Texture height */
        uint32 getHeight() const { return mHeight; }

        /** @return Texture depth [1 for 2D, 6 for Cube, 1 - N for 3D textures]*/
        uint32 getDepth() const { return mDepth; }

        /** @return True, if mip maps must be generated automatically */
        bool genMipMaps() const { return mGenMipMaps; }

        /** @return Texture type */
        ETextureType getTextureType() const { return mTextureType; }

        /** @return Texture internal storage format in RHI resource */
        EStorageFormat getStorageFormat() const { return mStorageFormat; }

    protected:

        uint32 mWidth;
        uint32 mHeight;
        uint32 mDepth;

        bool mGenMipMaps;
        ETextureType mTextureType;
        EStorageFormat mStorageFormat;

    };

} // namespace Berserk

#endif //BERSERK_TEXTURE_H