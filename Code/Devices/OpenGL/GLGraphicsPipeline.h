/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
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


        ~GLGraphicsPipeline() override = default;

        void create(const RHIGraphicsPipelineDesc& pipelineDesc) {
            BERSERK_COND_ERROR_FAIL(pipelineDesc.shader.isNotNull(), "Null pipeline shader");
            BERSERK_COND_ERROR_FAIL(pipelineDesc.framebufferFormat.colorFormats.size() > 0 || pipelineDesc.framebufferFormat.useDepthStencil, "Invalid framebuffer format");
            BERSERK_COND_ERROR_FAIL(pipelineDesc.framebufferFormat.colorFormats.size() == pipelineDesc.blendState.attachments.size(), "Incompatible framebuffer and blend state");

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
            if (mShader != desc.shader) return false;
            if (mPrimitivesType != desc.primitivesType) return false;
            if (mPolygonMode != desc.polygonMode) return false;
            if (mPolygonCullMode != desc.polygonCullMode) return false;
            if (mPolygonFrontFace != desc.polygonFrontFace) return false;
            if (mLineWidth != desc.lineWidth) return false;
            if (mDepthTest != desc.depthTest) return false;
            if (mDepthWrite != desc.depthWrite) return false;
            if (mDepthCompare != desc.depthCompare) return false;
            if (!(mBlendStateDesc == desc.blendState)) return false;
            if (!(mStencilStateDesc == desc.stencilState)) return false;
            if (!(mFramebufferDesc == desc.framebufferFormat)) return false;

            return true;
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
            pipeline->create(desc);
            mCachedGraphicsPipelines.add(pipeline);
            return pipeline;
        }

        static void clearCachedGraphicsPipelines() {
            mCachedGraphicsPipelines.clear();
        }

    private:

        /** GL general enums */
        GLState mStateGL;
        /** GL converted enums for blend attachments */
        TArrayStatic<GLBlendAttachment> mBlendAttachmentsGL;

        /** Cached graphics pipelines */
        static TArray<TPtrShared<GLGraphicsPipeline>> mCachedGraphicsPipelines;

    };

}



#endif //BERSERK_GLGRAPHICSPIPELINE_H