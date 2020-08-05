/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLGRAPHICSPIPELINE_H
#define BERSERK_GLGRAPHICSPIPELINE_H

#include <RHI/RHIResources.h>
#include <GLDefinitions.h>
#include <GLErrors.h>
#include <GLDevice.h>
#include <GLShader.h>

namespace Berserk {

    class GLGraphicsPipeline {
    public:

        struct GLBlendAttachment {
            GLboolean enable;
            GLenum srcColorBlendFactor;
            GLenum dstColorBlendFactor;
            GLenum colorBlendOp;
            GLenum srcAlphaBlendFactor;
            GLenum dstAlphaBlendFactor;
            GLenum alphaBlendOp;
        };

        struct GLState {
            GLenum polygonMode;
            GLenum polygonCullMode;
            GLenum frontFace;
            GLenum depthCompare;
        };

        struct GLStencil {
            bool enable = false;
            uint32 writeMask = 0;
            uint32 clearValue = 0;
            uint32 referenceValue = 0;
            uint32 compareMask = 0;
            GLenum compareFunction;
            GLenum sfail;
            GLenum dfail;
            GLenum dpass;
        };

        GLGraphicsPipeline() : GLGraphicsPipeline(RHIGraphicsPipelineState()) {
            // Default settings
        }

        GLGraphicsPipeline(const RHIGraphicsPipelineState& pipelineState) {
            BERSERK_COND_ERROR_RET(pipelineState.shader.isNotNull(), "Null pipeline shader");

            mShader = pipelineState.shader;
            mLineWidth = pipelineState.lineWidth;

            mDepthTest = pipelineState.depthTest;
            mDepthWrite = pipelineState.depthWrite;

            mStateGL.polygonMode = GLDefinitions::getPolygonMode(pipelineState.polygonMode);
            mStateGL.polygonCullMode = GLDefinitions::getPolygonCullMode(pipelineState.polygonCullMode);
            mStateGL.frontFace = GLDefinitions::getPolygonFrontFace(pipelineState.polygonFrontFace);
            mStateGL.depthCompare = GLDefinitions::getCompareFunc(pipelineState.depthCompare);

            auto& stencilState = pipelineState.stencilState;

            mStencil.enable = stencilState.enable;
            mStencil.compareMask = stencilState.compareMask;
            mStencil.compareFunction = GLDefinitions::getCompareFunc(stencilState.compareFunction);
            mStencil.writeMask = stencilState.writeMask;
            mStencil.referenceValue = stencilState.referenceValue;
            mStencil.sfail = GLDefinitions::getStencilOp(stencilState.sfail);
            mStencil.dfail = GLDefinitions::getStencilOp(stencilState.dfail);
            mStencil.dpass = GLDefinitions::getStencilOp(stencilState.dpass);

            auto& blendState = pipelineState.blendState;

            for (uint32 i = 0; i < blendState.attachments.size(); i++) {
                auto& attach = blendState.attachments[i];
                auto& GL_attach = mBlendAttachmentsGL.emplace();

                GL_attach.enable = attach.enable;
                GL_attach.alphaBlendOp = GLDefinitions::getBlendOperation(attach.alphaBlendOp);
                GL_attach.colorBlendOp = GLDefinitions::getBlendOperation(attach.colorBlendOp);
                GL_attach.srcAlphaBlendFactor = GLDefinitions::getBlendFactor(attach.srcAlphaBlendFactor);
                GL_attach.srcColorBlendFactor = GLDefinitions::getBlendFactor(attach.srcColorBlendFactor);
                GL_attach.dstAlphaBlendFactor = GLDefinitions::getBlendFactor(attach.dstAlphaBlendFactor);
                GL_attach.dstColorBlendFactor = GLDefinitions::getBlendFactor(attach.dstColorBlendFactor);

                mUsesBlending |= attach.enable;
            }
        }

        ~GLGraphicsPipeline() = default;


        void bind() const {
            // General pipeline setup
            glLineWidth(mLineWidth);
            glFrontFace(mStateGL.frontFace);
            glPolygonMode(GL_FRONT_AND_BACK, mStateGL.polygonMode);

            if (mStateGL.polygonCullMode != GL_NONE) {
                glEnable(GL_CULL_FACE);
                glCullFace(mStateGL.polygonCullMode);
            }
            else {
                glDisable(GL_CULL_FACE);
            }

            // Configure depth state
            if (mDepthTest)
                glEnable(GL_DEPTH_TEST);
            else
                glDisable(GL_DEPTH_TEST);

            if (mDepthWrite)
                glDepthMask(GL_TRUE);
            else
                glDepthMask(GL_FALSE);

            glDepthFunc(mStateGL.depthCompare);

            if (mStencil.enable) {
                glEnable(GL_STENCIL_TEST);
                glStencilMask(mStencil.writeMask);
                glStencilFunc(mStencil.compareFunction, mStencil.referenceValue, mStencil.compareMask);
                glStencilOp(mStencil.sfail, mStencil.dfail, mStencil.dpass);
            }
            else
                glDisable(GL_STENCIL_TEST);

            // Blend attachment configuration
            if (mUsesBlending) {
                glEnable(GL_BLEND);

                for (uint32 i = 0; i < mBlendAttachmentsGL.size(); i++) {
                    auto& GL_attachment = mBlendAttachmentsGL[i];

                    if (GL_attachment.enable) {
                        glEnablei(GL_BLEND, i);
                        glBlendEquationSeparatei(i, GL_attachment.colorBlendOp, GL_attachment.alphaBlendOp);
                        glBlendFuncSeparatei(i, GL_attachment.srcColorBlendFactor, GL_attachment.dstColorBlendFactor, GL_attachment.srcAlphaBlendFactor, GL_attachment.dstAlphaBlendFactor);
                    }
                    else {
                        glDisablei(GL_BLEND, i);
                    }
                }
            }
            else {
                glDisable(GL_BLEND);
            }

            // Attach actual shader
            if (mShader.isNotNull())
                glUseProgram(getShader().getProgramHandle());

            BERSERK_CATCH_OPENGL_ERRORS();
        }

        const GLState &getPipelineState() const {
            return mStateGL;
        }

        GLShader& getShader() const {
            return (GLShader&) *mShader;
        }

    private:

        TPtrShared<RHIShader> mShader;
        float mLineWidth;
        bool mDepthTest;
        bool mDepthWrite;
        bool mUsesBlending = false;

        /** GL general enums */
        GLState mStateGL;
        /** GL stencil state */
        GLStencil mStencil;
        /** GL converted enums for blend attachments */
        TArrayStatic<GLBlendAttachment> mBlendAttachmentsGL;

    };

}



#endif //BERSERK_GLGRAPHICSPIPELINE_H