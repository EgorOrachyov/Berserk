/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIFRAMEBUFFER_H
#define BERSERK_RHIFRAMEBUFFER_H

#include <RHI/RHIResource.h>

namespace Berserk {

    class RHIFramebuffer : public RHIResource {
    public:

        ~RHIFramebuffer() override = default;

        /** Set clear color for desired attachment */
        void setClearColor(uint32 attachment, const Color4f &color) { mClearColors[attachment] = color; }

        /** Set clear value for depth buffer */
        void setDepthClear(float value) { mDepthClear = value; }

        /** Set clear value for stencil buffer */
        void setStencilClear(uint32 value) { mStencilClear = value; }

        /** @return True, if this framebuffer uses depth stencil attachment*/
        bool useDepthStencil() const { return mDepthAttachment.isNotNull(); }

        /** @return Framebuffer width */
        uint32 getWidth() const { return mWidth; }

        /** @return Framebuffer height */
        uint32 getHeight() const { return mHeight; }

        /** @return Depth clear value */
        float getDepthClear() const { return mDepthClear; }

        /** @return Stencil clear value */
        int32 getStencilClear() const { return mStencilClear; }

        /** @return Clear colors for color attachments */
        const TArrayStatic<Color4f> &getClearColors() const { return mClearColors; }

        /** @return Color write buffers of the framebuffer (from 0 .. to N - 1)*/
        const TArrayStatic<TPtrShared<class RHITexture>> &getColorAttachments() const { return mColorAttachments; }

        /** @return Optional depth-stencil buffer (might be null) */
        const TPtrShared<RHITexture> &getDepthStencilAttachment() const { return mDepthAttachment; }

    protected:

        /** Framebuffer width */
        uint32 mWidth;

        /** Framebuffer height */
        uint32 mHeight;

        /** Depth buffer clear value */
        float mDepthClear = 1.0f;

        /** Stencil buffer clear value */
        int32 mStencilClear = 0;

        /** Clear colors for color attachments */
        TArrayStatic<Color4f,RHIConst::MAX_COLOR_ATTACHMENTS> mClearColors;

        /** Color write buffers of the framebuffer (from 0 .. to N - 1)*/
        TArrayStatic<TPtrShared<class RHITexture>,RHIConst::MAX_COLOR_ATTACHMENTS> mColorAttachments;

        /** Optional depth-stencil buffer (might be null) */
        TPtrShared<class RHITexture> mDepthAttachment;
    };

}

#endif //BERSERK_RHIFRAMEBUFFER_H
