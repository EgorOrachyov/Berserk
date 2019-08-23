//
// Created by Egor Orachyov on 2019-08-23.
//

#ifndef BERSERK_RENDERTARGET_H
#define BERSERK_RENDERTARGET_H

#include <Math/Rect.h>
#include <Strings/String.h>
#include <RHI/RHIResources.h>

namespace Berserk
{

    /** Target handles internal RHI frame buffer for rendering and processing scenes */
    class RENDER_API RenderTarget : public Allocatable
    {
    public:

        /**
         * Creates render target
         * @param name Descriptive name (text width x height @ N bits per pixel) or something else
         * @param viewport Full size viewport for rendering
         * @param isHDR True if buffer pixel format supports HDR rendering
         * @param frameBuffer Actual RHI resource
         */
        RenderTarget(String name, Rect viewport, bool isHDR, RHIFrameBufferRef frameBuffer)
            : mIsHDR(isHDR),
              mViewport(viewport),
              mName(std::move(name)),
              mFrameBuffer(std::move(frameBuffer))
        {}

        /** @return Target descriptive name for look-up */
        const String& getName() const { return mName; }

        /** @return Target full-size viewport */
        const Rect& getViewport() const { return mViewport; }

        /** @return True if supports HDR rendering */
        const bool supportsHDR() const { return mIsHDR; }

        /** @return RHI resource reference */
        const RHIFrameBufferRef& getBuffer() const { return mFrameBuffer; }

    protected:

        bool mIsHDR;
        Rect mViewport;
        String mName;
        RHIFrameBufferRef mFrameBuffer;

    };

} // namespace Berserk

#endif //BERSERK_RENDERTARGET_H