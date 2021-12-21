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

#ifndef BERSERK_GLRENDERPASS_HPP
#define BERSERK_GLRENDERPASS_HPP

#include <rhi/RHIRenderPass.hpp>
#include <rhi/opengl/GLDefs.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup opengl
 * @{
 */

/**
 * @class GLRenderPassStateVars
 * @brief GL render pass shared state variables for context management
 */
struct GLRenderPassStateVars {
    bool hasDepthAttachment;
    bool hasStencilAttachment;
    uint32 colorAttachmentsCount;
};

/**
 * @class GLRenderPass
 * @brief GL render pass state object
 */
class GLRenderPass final : public RHIRenderPass {
public:
    BRK_API explicit GLRenderPass(const RHIRenderPassDesc &desc);
    BRK_API ~GLRenderPass() override = default;

    BRK_API void Bind(GLRenderPassStateVars &state);
    BRK_API void BindFramebuffer(GLRenderPassStateVars &state);
    BRK_API void BindWindow(GLRenderPassStateVars &state);

    bool WindowPass() const { return mDesc.window.IsNotNull(); }
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_GLRENDERPASS_HPP
