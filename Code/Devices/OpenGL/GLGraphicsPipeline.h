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

    class GLGraphicsPipeline : public RHIGraphicsPipeline {
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
            GLenum primitivesType;
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


        ~GLGraphicsPipeline() override = default;

        bool create(const RHIGraphicsPipelineDesc& pipelineDesc) {
            BERSERK_COND_ERROR_RET_VALUE(false, pipelineDesc.shader.isNotNull(), "Null pipeline shader");
            BERSERK_COND_ERROR_RET_VALUE(false, pipelineDesc.framebufferFormat.colorFormats.size() > 0 || pipelineDesc.framebufferFormat.useDepthStencil, "Invalid framebuffer format");
            BERSERK_COND_ERROR_RET_VALUE(false, pipelineDesc.framebufferFormat.colorFormats.size() == pipelineDesc.blendState.attachments.size(), "Incompatible framebuffer and blend state");

            mShader = pipelineDesc.shader;
            mPrimitivesType = pipelineDesc.primitivesType;
            mPolygonMode = pipelineDesc.polygonMode;
            mPolygonCullMode = pipelineDesc.polygonCullMode;
            mPolygonFrontFace = pipelineDesc.polygonFrontFace;

            mLineWidth = pipelineDesc.lineWidth;

            mDepthTest = pipelineDesc.depthTest;
            mDepthWrite = pipelineDesc.depthWrite;
            mDepthCompare = pipelineDesc.depthCompare;

            mBlendStateDesc = pipelineDesc.blendState;
            mStencilStateDesc = pipelineDesc.stencilState;
            mFramebufferDesc = pipelineDesc.framebufferFormat;

            mStateGL.primitivesType = GLDefinitions::getPrimitivesType(mPrimitivesType);
            mStateGL.polygonMode = GLDefinitions::getPolygonMode(mPolygonMode);
            mStateGL.polygonCullMode = GLDefinitions::getPolygonCullMode(mPolygonCullMode);
            mStateGL.frontFace = GLDefinitions::getPolygonFrontFace(mPolygonFrontFace);
            mStateGL.depthCompare = GLDefinitions::getCompareFunc(mDepthCompare);

            mStencil.enable = mStencilStateDesc.enable;
            mStencil.compareMask = mStencilStateDesc.compareMask;
            mStencil.compareFunction = GLDefinitions::getCompareFunc(mStencilStateDesc.compareFunction);
            mStencil.writeMask = mStencilStateDesc.writeMask;
            mStencil.referenceValue = mStencilStateDesc.referenceValue;
            mStencil.sfail = GLDefinitions::getStencilOp(mStencilStateDesc.sfail);
            mStencil.dfail = GLDefinitions::getStencilOp(mStencilStateDesc.dfail);
            mStencil.dpass = GLDefinitions::getStencilOp(mStencilStateDesc.dpass);

            for (uint32 i = 0; i < mBlendStateDesc.attachments.size(); i++) {
                auto& attach = mBlendStateDesc.attachments[i];
                auto& GL_attach = mBlendAttachmentsGL.emplace();

                GL_attach.enable = attach.enable;
                GL_attach.alphaBlendOp = GLDefinitions::getBlendOperation(attach.alphaBlendOp);
                GL_attach.colorBlendOp = GLDefinitions::getBlendOperation(attach.colorBlendOp);
                GL_attach.srcAlphaBlendFactor = GLDefinitions::getBlendFactor(attach.srcAlphaBlendFactor);
                GL_attach.srcColorBlendFactor = GLDefinitions::getBlendFactor(attach.srcColorBlendFactor);
                GL_attach.dstAlphaBlendFactor = GLDefinitions::getBlendFactor(attach.dstAlphaBlendFactor);
                GL_attach.dstColorBlendFactor = GLDefinitions::getBlendFactor(attach.dstColorBlendFactor);
            }

            return true;
        }

        void bind() const {
            // General pipeline setup
            glPolygonMode(GL_FRONT_AND_BACK, mStateGL.polygonMode);
            if (mStateGL.polygonCullMode != GL_NONE) {
                glEnable(GL_CULL_FACE);
                glCullFace(mStateGL.polygonCullMode);
            }
            else {
                glDisable(GL_CULL_FACE);
            }
            glFrontFace(mStateGL.frontFace);
            glLineWidth(mLineWidth);

            // Configure depth state
            if (mDepthTest) {
                glEnable(GL_DEPTH_TEST);
            }
            else {
                glDisable(GL_DEPTH_TEST);
            }
            if (mDepthWrite) {
                glDepthMask(GL_TRUE);
            }
            else {
                glDepthMask(GL_FALSE);
            }
            glDepthFunc(mStateGL.depthCompare);

            if (mStencil.enable) {
                glEnable(GL_STENCIL_TEST);
                glStencilMask(mStencil.writeMask);
                glStencilFunc(mStencil.compareFunction, mStencil.referenceValue, mStencil.compareMask);
                glStencilOp(mStencil.sfail, mStencil.dfail, mStencil.dpass);
            }
            else {
                glDisable(GL_STENCIL_TEST);
            }

            // Blend attachment configuration
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

            // Attach actual shader
            glUseProgram(((GLShader&)*mShader).getProgramHandle());

            BERSERK_CATCH_OPENGL_ERRORS();
        }

        bool compatible(const RHIGraphicsPipelineDesc& desc) const {
            return (mShader == desc.shader)
                && (mPrimitivesType == desc.primitivesType)
                && (mPolygonMode == desc.polygonMode)
                && (mPolygonCullMode == desc.polygonCullMode)
                && (mPolygonFrontFace == desc.polygonFrontFace)
                && (mLineWidth == desc.lineWidth)
                && (mDepthTest == desc.depthTest)
                && (mDepthWrite == desc.depthWrite)
                && (mDepthCompare == desc.depthCompare)
                && (mBlendStateDesc == desc.blendState)
                && (mStencilStateDesc == desc.stencilState)
                && (mFramebufferDesc == desc.framebufferFormat);
        }

        const GLState &getPipelineState() const {
            return mStateGL;
        }

        static TPtrShared<GLGraphicsPipeline> createPipeline(const RHIGraphicsPipelineDesc &desc) {
            for (const auto& pipeline: mCachedGraphicsPipelines) {
                if (pipeline->compatible(desc)) {
                    return pipeline;
                }
            }

            auto pipeline = TPtrShared<GLGraphicsPipeline>::make();
            auto result = pipeline->create(desc);

            if (result) {
                mCachedGraphicsPipelines.add(pipeline);
                return pipeline;
            }

            return nullptr;
        }

        static void clearCachedGraphicsPipelines() {
            mCachedGraphicsPipelines.clear();
        }

    private:

        /** GL general enums */
        GLState mStateGL;
        /** GL stencil state */
        GLStencil mStencil;
        /** GL converted enums for blend attachments */
        TArrayStatic<GLBlendAttachment> mBlendAttachmentsGL;

        /** Cached graphics pipelines */
        static TArray<TPtrShared<GLGraphicsPipeline>> mCachedGraphicsPipelines;

    };

}



#endif //BERSERK_GLGRAPHICSPIPELINE_H