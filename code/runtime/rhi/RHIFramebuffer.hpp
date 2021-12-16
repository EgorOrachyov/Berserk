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

#ifndef BERSERK_RHIFRAMEBUFFER_HPP
#define BERSERK_RHIFRAMEBUFFER_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/string/StringName.hpp>
#include <rhi/RHIResource.hpp>
#include <rhi/RHITexture.hpp>

#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup rhi
 * @{
 */

/**
 * @class RHIAttachmentDesc
 * @brief Describes single render target attachment
 */
struct RHIAttachmentDesc {
    Ref<RHITexture> target;
    uint32 arraySlice = 0;
    uint32 face = 0;
    uint32 mipLevel = 0;
};

/**
 * @class RHIFramebufferDesc
 * @brief RHI framebuffer descriptor
 */
struct RHIFramebufferDesc {
    StringName name;
    uint32 width;
    uint32 height;
    RHIAttachmentDesc depthStencilTarget;
    std::vector<RHIAttachmentDesc> colorTargets;
};

/**
 * @class RHIFramebuffer
 * @brief Collection of render targets for drawing
 */
class RHIFramebuffer : public RHIResource {
public:
    BRK_API ~RHIFramebuffer() override = default;

    /** @return Framebuffer width in pixels */
    uint32 GetWidth() const { return mDesc.width; }

    /** @return Framebuffer height in pixels */
    uint32 GetHeight() const { return mDesc.height; }

    /** @return Number of color attachments in this framebuffer */
    size_t GetColorAttachmentsCount() const { return mDesc.colorTargets.size(); }

    /** @return Framebuffer debug name (displayed in the vk debugger) */
    const StringName &GetName() const { return mDesc.name; }

    /** @return Render target desc */
    const RHIFramebufferDesc &GetDesc() const { return mDesc; }

protected:
    /** Render target desc */
    RHIFramebufferDesc mDesc;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RHIFRAMEBUFFER_HPP
