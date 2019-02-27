//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_IFRAMEBUFFER_H
#define BERSERK_IFRAMEBUFFER_H

#include "Misc/Bits.h"
#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"
#include "Resource/IResource.h"
#include "Platform/IRenderDriver.h"

namespace Berserk
{

    /**
     * Abstract platform driver layer for frame buffer target
     */
    class GRAPHICS_API IFrameBuffer : public IResource
    {
    public:

        /** Creates frame buffer of chosen size */
        virtual void createFrameBuffer(uint32 width, uint32 height, uint32 colorAttachments) = 0;

        /** Add color attachemnt to frame buffer */
        virtual void attachColorBuffer(IRenderDriver::StorageFormat format) = 0;

        /** Add depth attachemnt to frame buffer */
        virtual void attachDepthBuffer() = 0;

        /** Add depth and stencil attachement to frame buffer */
        virtual void attachDepthStencilBuffer() = 0;

        /** Link all the attached buffers */
        virtual void linkBuffers() = 0;

        /** Bind frame buffer - use as render target */
        virtual void bindFrameBuffer() = 0;

        /** Uniform color buffer - layer: which attachment, slot: target uniform texture*/
        virtual void bindColorBuffer(uint32 layer, uint32 textureSlot) = 0;

        /** Uniform depth buffer to texture slot */
        virtual void bindDepthBuffer(uint32 textureSlot) = 0;

        /** Uniform depth and stencil buffer to texture slot */
        virtual void bindDepthStencilBuffer(uint32 textureSlot) = 0;

        /** @return Memory used at gpu side */
        virtual uint32 getGPUMemoryUsage() = 0;

        /** @return Number of max color attachments to the frame buffer */
        virtual uint32 getMaxSupportedColorBuffers() = 0;

    };

} // namespace Berserk

#endif //BERSERK_IFRAMEBUFFER_H