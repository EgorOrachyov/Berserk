//
// Created by Egor Orachyov on 2019-08-09.
//

#include <Rendering/Texture.h>

namespace Berserk
{

    Texture::Texture(Berserk::uint32 width, Berserk::uint32 height, Berserk::uint32 depth, bool genMipMaps,
                     Berserk::ETextureType textureType, Berserk::EStorageFormat storageFormat)
         : mWidth(width),
           mHeight(height),
           mDepth(depth),
           mGenMipMaps(genMipMaps),
           mTextureType(textureType),
           mStorageFormat(storageFormat)

    {

    }

} // namespace Berserk
