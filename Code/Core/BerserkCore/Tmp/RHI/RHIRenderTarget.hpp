/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIRENDERTARGET_H
#define BERSERK_RHIRENDERTARGET_H

#include <BerserkCore/RHI/RHIDefinitions.hpp>
#include <BerserkCore/RHI/RHITexture.hpp>
#include <BerserkCore/RHI/RHIResource.hpp>
#include <BerserkCore/Templates/ArrayFixed.hpp>
#include <BerserkCore/TPtrShared.hpp>

namespace Berserk {

    using RHIColorBuffers = TArrayFixed<TPtrShared<RHITexture>,RHI::MAX_COLOR_BUFFER_COUNT>;

    class RHIRenderTarget: public RHIResource {
    public:
        ~RHIRenderTarget() override = default;
        const RHIColorBuffers& GetColorBuffers() const { return mColorBuffers; }
        const TPtrShared<RHITexture>& GetDepthStencilBuffer() const { return mDepthStencilBuffer; }
        uint32 GetWidth() const { return mWidth; }
        uint32 GetHeight() const { return mHeight; }
        bool HasColorBuffers() const { return mColorBuffers.IsNotEmpty(); }
        bool HasDepthStencilBuffer() const { return mDepthStencilBuffer.IsNotNull(); }

    protected:
        RHIColorBuffers mColorBuffers;
        TPtrShared<RHITexture> mDepthStencilBuffer;
        uint32 mWidth;
        uint32 mHeight;
    };

}

#endif //BERSERK_RHIRENDERTARGET_H