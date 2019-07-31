//
// Created by Egor Orachyov on 2019-07-11.
//

#ifndef BERSERK_RHIFRAMEBUFFER_H
#define BERSERK_RHIFRAMEBUFFER_H

#include <RHI/RHIResource.h>
#include <RHI/RHITexture.h>

namespace Berserk
{

    class GRAPHICS_API RHIFrameBuffer : public RHIResource
    {
    public:

        virtual ~RHIFrameBuffer() = default;

        /** Binds this frame buffer as active buffer */
        virtual void bind() = 0;

        /** @return Number of color attachments to this frame buffer */
        virtual uint32 getColorAttachmentsCount() const = 0;

        /** @return Depth buffer texture (or null, if is not attached) */
        virtual RHITextureRef getDepthAttachment() const = 0;

        /** @return Color buffer texture (or null, if is not attached) */
        virtual RHITextureRef getColorAttachment(uint32 layer) const = 0;

    };

    typedef TSharedPtr<RHIFrameBuffer> RHIFrameBufferRef;

} // namespace Berserk

#endif //BERSERK_RHIFRAMEBUFFER_H