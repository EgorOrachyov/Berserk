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

#ifndef BERSERK_RHIRENDERPASS_HPP
#define BERSERK_RHIRENDERPASS_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <platform/Window.hpp>
#include <rhi/RHIFramebuffer.hpp>
#include <rhi/RHIResource.hpp>

#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup rhi
 * @{
 */

/**
 * @class RHIColorAttachment
 * @brief Single render target color attachment descriptor
 */
struct RHIColorAttachment {
    Vec4f clearColor{};
    RHIRenderTargetOption option = RHIRenderTargetOption::DiscardDiscard;
};

/**
 * @class RHIDepthStencilAttachment
 * @brief Depth-stencil render target attachment descriptor
 */
struct RHIDepthStencilAttachment {
    float depthClear = 1.0f;
    uint32 stencilClear = 0;
    RHIRenderTargetOption depthOption = RHIRenderTargetOption::DiscardDiscard;
    RHIRenderTargetOption stencilOption = RHIRenderTargetOption::DiscardDiscard;
};

/**
 * @class RHIViewport
 * @brief Area of the render target to draw
 */
struct RHIViewport {
    int32 left = 0;
    int32 bottom = 0;
    uint32 width = 0;
    uint32 height = 0;
};

/**
 * @class RHIRenderPassDesc
 * @brief Full render pass descriptor
 */
class RHIRenderPassDesc {
public:
    /** Action to perform on each color attachment before/after pass */
    std::vector<RHIColorAttachment> colorAttachments;

    /** Action to perform on each depth/stencil attachment before/after pass */
    RHIDepthStencilAttachment depthStencilAttachment;

    /** Region to draw into */
    RHIViewport viewport;

    /** Name of the render pass for debugging */
    StringName name;

    /** For offscreen rendering */
    Ref<RHIFramebuffer> framebuffer;

    /** For window rendering */
    Ref<Window> window;
};

/**
 * @class RHIRenderPass
 * @brief Compiled render pass object
 */
class RHIRenderPass : public RHIResource {
public:
    BRK_API ~RHIRenderPass() override = default;

    /** @return Full render pass description */
    const RHIRenderPassDesc &GetDesc() const { return mDesc; }

protected:
    /** Full render pass description */
    RHIRenderPassDesc mDesc;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RHIRENDERPASS_HPP
