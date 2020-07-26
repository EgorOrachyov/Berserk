/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RenderResources/GraphicsPipelineBuilder.h>
#include <RenderResources/GraphicsPipeline.h>
#include <RenderResources/VertexDeclaration.h>
#include <RenderTargets/RenderTarget.h>
#include <ShaderCore/ShaderProgram.h>

namespace Berserk {
    namespace Render {

        GraphicsPipelineBuilder& GraphicsPipelineBuilder::setTarget(const struct RenderTarget &target) {
            mTargetHasColor = target.supportsColorBuffer();
            mTargetHasDepth = target.supportsDepthBuffer();
            mTargetHasStencil = target.supportsStencilBuffer();
            mTargetOffscreen = target.isOffscreenTarget();

            target.extractFormat(mPipelineDesc.framebufferFormat);
            target.extractDefaultBlendState(mPipelineDesc.blendState);

            mPipelineDesc.forWindowRendering = !mTargetOffscreen;

            return *this;
        }

        GraphicsPipelineBuilder& GraphicsPipelineBuilder::setShader(const TPtrShared<ShaderProgram> &shader) {
            BERSERK_COND_ERROR_RET_VALUE(*this, shader.isNotNull(), "Passed null shader");
            mShaderProgram = shader;
            mPipelineDesc.shader = mShaderProgram->getShader();
            return *this;
        }

        GraphicsPipelineBuilder& GraphicsPipelineBuilder::setDeclaration(const TPtrShared<VertexDeclaration> &declaration) {
            BERSERK_COND_ERROR_RET_VALUE(*this, declaration.isNotNull(), "Passed null declaration");
            mVertexDeclaration = declaration;
            mPipelineDesc.vertexDeclaration = declaration->getRHI();
            return *this;
        }

        GraphicsPipelineBuilder & GraphicsPipelineBuilder::primitivesType(EPrimitivesType primitivesType) {
            mPipelineDesc.primitivesType = primitivesType;
            return *this;
        }

        GraphicsPipelineBuilder & GraphicsPipelineBuilder::polygonMode(EPolygonMode polygonMode) {
            mPipelineDesc.polygonMode = polygonMode;
            return *this;
        }

        GraphicsPipelineBuilder & GraphicsPipelineBuilder::polygonCullMode(EPolygonCullMode cullMode) {
            mPipelineDesc.polygonCullMode = cullMode;
            return *this;
        }

        GraphicsPipelineBuilder & GraphicsPipelineBuilder::polygonFrontFace(EPolygonFrontFace frontFace) {
            mPipelineDesc.polygonFrontFace = frontFace;
            return *this;
        }

        GraphicsPipelineBuilder& GraphicsPipelineBuilder::lineWidth(float lineWidth) {
            mPipelineDesc.lineWidth = lineWidth;
            return *this;
        }
        
        GraphicsPipelineBuilder& GraphicsPipelineBuilder::depthTest(bool enable) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mTargetHasDepth, "Target does not support depth buffer");
            mPipelineDesc.depthTest = enable;
            return *this;
        }

        GraphicsPipelineBuilder& GraphicsPipelineBuilder::depthWrite(bool enable) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mTargetHasDepth, "Target does not support depth buffer");
            mPipelineDesc.depthWrite = enable;
            return *this;
        }

        GraphicsPipelineBuilder & GraphicsPipelineBuilder::depthFunction(ECompareFunction compareFunction) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mTargetHasDepth, "Target does not support depth buffer");
            mPipelineDesc.depthCompare = compareFunction;
            return *this;
        }

        GraphicsPipelineBuilder& GraphicsPipelineBuilder::stencilTest(bool enable) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mTargetHasStencil, "Target does not support stencil buffer");
            mPipelineDesc.stencilState.enable = enable;
            return *this;
        }

        GraphicsPipelineBuilder & GraphicsPipelineBuilder::stencilWriteMask(uint32 mask) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mTargetHasStencil, "Target does not support stencil buffer");
            mPipelineDesc.stencilState.writeMask = mask;
            return *this;
        }

        GraphicsPipelineBuilder & GraphicsPipelineBuilder::stencilCompare(ECompareFunction compareFunction, uint32 reference, uint32 mask) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mTargetHasStencil, "Target does not support stencil buffer");
            mPipelineDesc.stencilState.compareFunction = compareFunction;
            mPipelineDesc.stencilState.referenceValue = reference;
            mPipelineDesc.stencilState.compareMask = mask;
            return *this;
        }

        GraphicsPipelineBuilder & GraphicsPipelineBuilder::stencilOp(EOperation sfail, EOperation dfail, EOperation dpass) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mTargetHasStencil, "Target does not support stencil buffer");
            mPipelineDesc.stencilState.sfail = sfail;
            mPipelineDesc.stencilState.dfail = dfail;
            mPipelineDesc.stencilState.dpass = dpass;
            return *this;
        }

        GraphicsPipelineBuilder& GraphicsPipelineBuilder::blend(bool enable) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mTargetHasColor, "Target does not support color buffers");
            for (auto& attachment: mPipelineDesc.blendState.attachments) {
                attachment.enable = enable;
            }
            return *this;
        }

        TPtrShared<class GraphicsPipeline> GraphicsPipelineBuilder::buildShared() {
            if (mInstance.isNotNull())
                return mInstance;

            mInstance = TPtrShared<GraphicsPipeline>::make(*this);
            return mInstance;
        }

    }
}