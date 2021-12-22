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

#include <rhi/opengl/GLGraphicsPipeline.hpp>
#include <rhi/opengl/GLRenderPass.hpp>
#include <rhi/opengl/GLShader.hpp>

BRK_NS_BEGIN

GLGraphicsPipeline::GLGraphicsPipeline(const RHIGraphicsPipelineDesc &desc) {
    mDesc = desc;
}

void GLGraphicsPipeline::Bind(const GLRenderPassStateVars &state) {
    auto &pipelineState = mDesc;

    assert(pipelineState.shader.IsNotNull());
    assert(pipelineState.declaration.IsNotNull());
    assert(pipelineState.shader->GetCompilationStatus() == RHIShader::Status::Compiled);

    // Bind shader for drawing
    pipelineState.shader.ForceCast<GLShader>()->Use();

    glBindVertexArray(0);
    BRK_GL_CATCH_ERR();

    // Setup the rest of the params
    auto &rstrState = pipelineState.rasterState;

    glLineWidth(rstrState.lineWidth);
    BRK_GL_CATCH_ERR();

    glFrontFace(GLDefs::GetPolygonFrontFace(rstrState.frontFace));
    BRK_GL_CATCH_ERR();

    glPolygonMode(GL_FRONT_AND_BACK, GLDefs::GetPolygonMode(rstrState.mode));
    BRK_GL_CATCH_ERR();

    if (rstrState.cullMode != RHIPolygonCullMode::Disabled) {
        glEnable(GL_CULL_FACE);
        BRK_GL_CATCH_ERR();

        glCullFace(GLDefs::GetPolygonCullMode(rstrState.cullMode));
        BRK_GL_CATCH_ERR();
    } else {
        glDisable(GL_CULL_FACE);
        BRK_GL_CATCH_ERR();
    }

    auto &dpstState = pipelineState.depthStencilState;

    if (state.hasDepthAttachment && dpstState.depthEnable) {
        glEnable(GL_DEPTH_TEST);
        BRK_GL_CATCH_ERR();

        glDepthMask(dpstState.depthWrite ? GL_TRUE : GL_FALSE);
        BRK_GL_CATCH_ERR();

        glDepthFunc(GLDefs::GetCompareFunc(dpstState.depthCompare));
        BRK_GL_CATCH_ERR();
    } else {
        glDisable(GL_DEPTH_TEST);
        BRK_GL_CATCH_ERR();
    }

    if (state.hasStencilAttachment && dpstState.stencilEnable) {
        glEnable(GL_STENCIL_TEST);
        BRK_GL_CATCH_ERR();

        glStencilMask(dpstState.writeMask);
        BRK_GL_CATCH_ERR();

        glStencilFunc(GLDefs::GetCompareFunc(dpstState.compareFunction), dpstState.referenceValue, dpstState.compareMask);
        BRK_GL_CATCH_ERR();

        glStencilOp(GLDefs::GetStencilOp(dpstState.sfail), GLDefs::GetStencilOp(dpstState.dfail), GLDefs::GetStencilOp(dpstState.dpass));
        BRK_GL_CATCH_ERR();
    } else {
        glDisable(GL_STENCIL_TEST);
        BRK_GL_CATCH_ERR();
    }

    auto &bldState = pipelineState.blendState;
    assert(bldState.attachments.size() == state.colorAttachmentsCount);

    auto enableBlend = false;

    for (uint32 i = 0; i < bldState.attachments.size(); i++) {
        auto &attach = bldState.attachments[i];

        if (attach.enable) {
            enableBlend = true;
            glBlendEquationSeparatei(i,
                                     GLDefs::GetBlendOperation(attach.colorBlendOp),
                                     GLDefs::GetBlendOperation(attach.alphaBlendOp));
            BRK_GL_CATCH_ERR();

            glBlendFuncSeparatei(i,
                                 GLDefs::GetBlendFactor(attach.srcColorBlendFactor),
                                 GLDefs::GetBlendFactor(attach.dstColorBlendFactor),
                                 GLDefs::GetBlendFactor(attach.srcAlphaBlendFactor),
                                 GLDefs::GetBlendFactor(attach.dstAlphaBlendFactor));
            BRK_GL_CATCH_ERR();
        }
    }

    if (enableBlend) {
        glEnable(GL_BLEND);
        BRK_GL_CATCH_ERR();
    } else {
        glDisable(GL_BLEND);
        BRK_GL_CATCH_ERR();
    }
}

BRK_NS_END