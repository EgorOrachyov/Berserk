//
// Created by Egor Orachyov on 28.02.2019.
//

#ifndef BERSERK_IDEPTHBUFFER_H
#define BERSERK_IDEPTHBUFFER_H

#include "Misc/Bits.h"
#include "Misc/Types.h"
#include "Resource/IResource.h"
#include "Misc/UsageDescriptors.h"
#include "Platform/IRenderDriver.h"

namespace Berserk
{

    class GRAPHICS_API IDepthBuffer : public IResource
    {
    public:

        /** Creates depth attachemnt to frame buffer */
        virtual void createDepthBuffer(uint32 width, uint32 height) = 0;

        /** Creates depth and stencil attachement to frame buffer */
        virtual void createDepthStencilBuffer() = 0;

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

        /** Get frame buffer width and height in pixels */
        virtual void getSize(uint32 &width, uint32 &height) = 0;

        /** @return Memory used at gpu side */
        virtual uint32 getGPUMemoryUsage() = 0;

    };

} // namespace Berserk

#endif //BERSERK_IDEPTHBUFFER_H