/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
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