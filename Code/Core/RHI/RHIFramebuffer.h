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

        /**  Limit somehow color attachment count */
        static const uint32 MAX_COLOR_ATTACHMENTS = 16;

        /** Set clear color for desired attachment */
        void setClearColor(uint32 attachment, const Color4f &color) { mClearColors[attachment] = color; }

        /** Set clear value for depth buffer */
        void setDepthClear(float value) { mDepthClear = value; }

        /** Set clear value for stencil buffer */
        void setStencilClear(uint32 value) { mStencilClear = value; }

        /** Set framebuffer clear option */
        void setClearOption(EClearOption option, bool value) { mClearOptions.setFlag(option, value); }

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

        /** @return Options on framebuffer clear */
        const TEnumMask<EClearOption> &getClearOptions() const { return mClearOptions; }

        /** @return Clear colors for color attachments */
        const TArray<Color4f> &getClearColors() const { return mClearColors; }

        /** @return Color write buffers of the framebuffer (from 0 .. to N - 1)*/
        const TArray<TPtrShared<RHITexture>> &getColorAttachments() const { return mColorAttachments; }

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

        /** Options on framebuffer clear */
        TEnumMask<EClearOption> mClearOptions;

        /** Clear colors for color attachments */
        TArray<Color4f> mClearColors;

        /** Color write buffers of the framebuffer (from 0 .. to N - 1)*/
        TArray<TPtrShared<RHITexture>> mColorAttachments;

        /** Optional depth-stencil buffer (might be null) */
        TPtrShared<RHITexture> mDepthAttachment;
    };

}

#endif //BERSERK_RHIFRAMEBUFFER_H
