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

#include <core/Engine.hpp>
#include <rhi/opengl/GLDevice.hpp>
#include <rhi/opengl/GLFramebuffer.hpp>
#include <rhi/opengl/GLRenderPass.hpp>

BRK_NS_BEGIN

GLRenderPass::GLRenderPass(const RHIRenderPassDesc &desc) {
    assert(desc.window.IsNotNull() || desc.framebuffer.IsNotNull());
    mDesc = desc;
}

void GLRenderPass::Bind(GLRenderPassStateVars &state) {
    if (WindowPass())
        BindWindow(state);
    else
        BindFramebuffer(state);
}

void GLRenderPass::BindFramebuffer(GLRenderPassStateVars &state) {
    auto &renderPass = mDesc;
    auto &renderTarget = renderPass.framebuffer;

    assert(renderTarget.IsNotNull());
    assert(renderPass.colorAttachments.size() == renderTarget->GetColorAttachmentsCount());

    auto colorAttachments = renderTarget->GetColorAttachmentsCount();
    auto target = renderTarget.ForceCast<GLFramebuffer>();

    glBindFramebuffer(GL_FRAMEBUFFER, target->GetHandle());
    BRK_GL_CATCH_ERR();

    auto viewport = renderPass.viewport;

    glViewport(viewport.left, viewport.bottom, static_cast<GLsizei>(viewport.width), static_cast<GLsizei>(viewport.height));
    BRK_GL_CATCH_ERR();

    // Clear target buffers if need
    for (uint32 i = 0; i < colorAttachments; i++) {
        if (GLDefs::NeedClearBefore(renderPass.colorAttachments[i].option)) {
            auto &c = renderPass.colorAttachments[i].clearColor;

            glDisable(GL_STENCIL_TEST);
            BRK_GL_CATCH_ERR();

            glColorMaski(i, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            BRK_GL_CATCH_ERR();

            glClearBufferfv(GL_COLOR, static_cast<int>(i), c.GetData());
            BRK_GL_CATCH_ERR();
        }
    }

    auto hasDepth = target->HasDepthBuffer();
    auto needClearDepth = hasDepth && GLDefs::NeedClearBefore(renderPass.depthStencilAttachment.depthOption);
    auto hasStencil = target->HasStencilBuffer();
    auto needClearStencil = hasStencil && GLDefs::NeedClearBefore(renderPass.depthStencilAttachment.stencilOption);

    if (needClearDepth || needClearStencil) {
        // Enable depth writing if it not
        glDepthMask(GL_TRUE);
        BRK_GL_CATCH_ERR();

        // Enable stencil write
        glStencilMask(0xffffffff);
        BRK_GL_CATCH_ERR();

        if (needClearDepth && needClearStencil) {
            auto stencilClear = static_cast<int32>(renderPass.depthStencilAttachment.stencilClear);
            glClearBufferfi(GL_DEPTH_STENCIL, 0, renderPass.depthStencilAttachment.depthClear, stencilClear);
            BRK_GL_CATCH_ERR();
        } else if (needClearDepth) {
            glClearBufferfv(GL_DEPTH, 0, &renderPass.depthStencilAttachment.depthClear);
            BRK_GL_CATCH_ERR();
        } else {
            auto stencilClear = static_cast<int32>(renderPass.depthStencilAttachment.stencilClear);
            glClearBufferiv(GL_DEPTH, 0, &stencilClear);
            BRK_GL_CATCH_ERR();
        }
    }

    state.colorAttachmentsCount = static_cast<uint32>(colorAttachments);
    state.hasDepthAttachment = hasDepth;
    state.hasStencilAttachment = hasStencil;
}

void GLRenderPass::BindWindow(GLRenderPassStateVars &state) {
    auto &renderPass = mDesc;
    auto &window = renderPass.window;

    assert(renderPass.colorAttachments.size() == 1);

    // Make context of the window current, so render pass drawing ends-up in this window
    auto device = dynamic_cast<GLDevice *>(&Engine::Instance().GetRHIDevice());
    device->GetContextFunc()(window);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    BRK_GL_CATCH_ERR();

    auto viewport = renderPass.viewport;

    glViewport(viewport.left, viewport.bottom, static_cast<GLsizei>(viewport.width), static_cast<GLsizei>(viewport.height));
    BRK_GL_CATCH_ERR();

    // Clear target buffers if need
    GLbitfield clearMask = 0;

    if (GLDefs::NeedClearBefore(renderPass.colorAttachments[0].option)) {
        auto &c = renderPass.colorAttachments[0].clearColor;
        clearMask |= GL_COLOR_BUFFER_BIT;

        glDisable(GL_STENCIL_TEST);
        BRK_GL_CATCH_ERR();

        glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        BRK_GL_CATCH_ERR();

        glClearColor(c.x(), c.y(), c.z(), c.w());
        BRK_GL_CATCH_ERR();
    }
    if (GLDefs::NeedClearBefore(renderPass.depthStencilAttachment.depthOption)) {
        clearMask |= GL_DEPTH_BUFFER_BIT;

        glDepthMask(GL_TRUE);
        BRK_GL_CATCH_ERR();

        glClearDepthf(renderPass.depthStencilAttachment.depthClear);
        BRK_GL_CATCH_ERR();
    }
    if (GLDefs::NeedClearBefore(renderPass.depthStencilAttachment.stencilOption)) {
        clearMask |= GL_STENCIL_BUFFER_BIT;

        glStencilMask(0xffffffff);
        BRK_GL_CATCH_ERR();

        glClearStencil(static_cast<int32>(renderPass.depthStencilAttachment.stencilClear));
        BRK_GL_CATCH_ERR();
    }

    if (clearMask != 0) {
        glClear(clearMask);
        BRK_GL_CATCH_ERR();
    }

    state.colorAttachmentsCount = 1;
    state.hasDepthAttachment = true;
    state.hasStencilAttachment = true;
}

BRK_NS_END