//
// Created by Egor Orachyov on 2019-07-11.
//

#ifndef BERSERK_RHITEXTURE_H
#define BERSERK_RHITEXTURE_H

#include <RHI/RHIResource.h>
#include <Rendering/Definitions.h>
#include <RHI/RHISampler.h>

namespace Berserk
{

    class GRAPHICS_API RHITexture : public RHIResource
    {
    public:

        virtual ~RHITexture() = default;

        /** Bind this texture in specified shader slot */
        virtual void bind(uint32 textureSlot) const = 0;

        /** Bind this texture in specified shader slot with chosen sampler */
        virtual void bind(uint32 textureSlot, const RHISamplerRef& sampler) = 0;

        /** Read texture content with specified format in the destination buffer */
        virtual void readData(EPixelFormat format, EDataType type, uint8 *destBuffer) const = 0;

        /** Read texture content LOD level with specified format in the destination buffer */
        virtual void readData(EPixelFormat format, EDataType type, uint32 lod, uint8 *destBuffer) const = 0;

        /** @return True, whether this texture uses mipmaps (LOD) */
        virtual bool isMipmapsUsed() const = 0;

        /** @return Storage format of this texture */
        virtual EStorageFormat getStorageFormat() const = 0;

    };

    typedef TSharedPtr<RHITexture> RHITextureRef;

} // namespace Berserk

#endif //BERSERK_RHITEXTURE_H