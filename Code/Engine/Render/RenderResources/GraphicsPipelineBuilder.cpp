/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RenderResources/GraphicsPipelineBuilder.h>
#include <RenderResources/VertexDeclaration.h>
#include <RenderTargets/RenderTarget.h>
#include <ShaderProgram.h>

namespace Berserk {
    namespace Render {

        GraphicsPipelineBuilder& GraphicsPipelineBuilder::setShader(const TPtrShared<ShaderProgram> &shader) {
            BERSERK_COND_ERROR_RET_VALUE(*this, shader.isNotNull(), "Passed null shader");
            mShaderProgram = shader;
            mPipelineState.shader = mShaderProgram->getShader();
            return *this;
        }

        GraphicsPipelineBuilder& GraphicsPipelineBuilder::setDeclaration(const TPtrShared<VertexDeclaration> &declaration) {
            BERSERK_COND_ERROR_RET_VALUE(*this, declaration.isNotNull(), "Passed null declaration");
            mVertexDeclaration = declaration;
            mPipelineState.vertexDeclaration = declaration->getRHI();
            return *this;
        }

        GraphicsPipelineBuilder & GraphicsPipelineBuilder::polygonMode(EPolygonMode polygonMode) {
            mPipelineState.polygonMode = polygonMode;
            return *this;
        }

        GraphicsPipelineBuilder & GraphicsPipelineBuilder::polygonCullMode(EPolygonCullMode cullMode) {
            mPipelineState.polygonCullMode = cullMode;
            return *this;
        }

        GraphicsPipelineBuilder & GraphicsPipelineBuilder::polygonFrontFace(EPolygonFrontFace frontFace) {
            mPipelineState.polygonFrontFace = frontFace;
            return *this;
        }

        GraphicsPipelineBuilder& GraphicsPipelineBuilder::lineWidth(float lineWidth) {
            mPipelineState.lineWidth = lineWidth;
            return *this;
        }
        
        GraphicsPipelineBuilder& GraphicsPipelineBuilder::depthTest(bool enable) {
            mPipelineState.depthTest = enable;
            return *this;
        }

        GraphicsPipelineBuilder& GraphicsPipelineBuilder::depthWrite(bool enable) {
            mPipelineState.depthWrite = enable;
            return *this;
        }

        GraphicsPipelineBuilder & GraphicsPipelineBuilder::depthFunction(ECompareFunction compareFunction) {
            mPipelineState.depthCompare = compareFunction;
            return *this;
        }

        GraphicsPipelineBuilder& GraphicsPipelineBuilder::stencilTest(bool enable) {
            mPipelineState.stencilState.enable = enable;
            return *this;
        }

        GraphicsPipelineBuilder & GraphicsPipelineBuilder::stencilWriteMask(uint32 mask) {
            mPipelineState.stencilState.writeMask = mask;
            return *this;
        }

        GraphicsPipelineBuilder & GraphicsPipelineBuilder::stencilCompare(ECompareFunction compareFunction, uint32 reference, uint32 mask) {
            mPipelineState.stencilState.compareFunction = compareFunction;
            mPipelineState.stencilState.referenceValue = reference;
            mPipelineState.stencilState.compareMask = mask;
            return *this;
        }

        GraphicsPipelineBuilder & GraphicsPipelineBuilder::stencilOp(EOperation sfail, EOperation dfail, EOperation dpass) {
            mPipelineState.stencilState.sfail = sfail;
            mPipelineState.stencilState.dfail = dfail;
            mPipelineState.stencilState.dpass = dpass;
            return *this;
        }

        GraphicsPipelineBuilder& GraphicsPipelineBuilder::blend(bool enable) {
            for (auto& attachment: mPipelineState.blendState.attachments) {
                attachment.enable = enable;
            }
            return *this;
        }

        GraphicsPipelineBuilder::Builder & GraphicsPipelineBuilder::blend(uint32 attachment, EBlendOperation alphaBlendOp, EBlendOperation colorBlendOp, EBlendFactor srcAlpha, EBlendFactor srcColor,EBlendFactor dstAlpha,EBlendFactor dstColor) {
            if (mPipelineState.blendState.attachments.size() <= attachment) {
                mPipelineState.blendState.attachments.resize(attachment + 1);
            }

            RHIBlendAttachment& desc = mPipelineState.blendState.attachments[attachment];
            desc.enable = true;
            desc.alphaBlendOp = alphaBlendOp;
            desc.colorBlendOp = colorBlendOp;
            desc.srcAlphaBlendFactor = srcAlpha;
            desc.srcColorBlendFactor = srcColor;
            desc.dstAlphaBlendFactor = dstAlpha;
            desc.dstColorBlendFactor = dstColor;

            return *this;
        }

        RHIGraphicsPipelineState GraphicsPipelineBuilder::build() {
            return mPipelineState;
        }

    }
}