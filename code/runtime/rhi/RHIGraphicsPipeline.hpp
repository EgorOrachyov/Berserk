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

#ifndef BERSERK_RHIGRAPHICSPIPELINE_HPP
#define BERSERK_RHIGRAPHICSPIPELINE_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <rhi/RHIRenderPass.hpp>
#include <rhi/RHIResource.hpp>
#include <rhi/RHIShader.hpp>
#include <rhi/RHIVertexDeclaration.hpp>

#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup rhi
 * @{
 */

/**
 * @class RHIRasterState
 * @brief RHI raster state descriptor
 */
struct RHIRasterState {
    RHIPolygonMode mode : 2;
    RHIPolygonCullMode cullMode : 3;
    RHIPolygonFrontFace frontFace : 3;
    uint8 lineWidth;

    RHIRasterState() {
        Memory::Set(this, 0x0, sizeof(RHIRasterState));
        mode = RHIPolygonMode::Fill;
        cullMode = RHIPolygonCullMode::Disabled;
        frontFace = RHIPolygonFrontFace::CounterClockwise;
        lineWidth = 1;
    }

    bool operator==(const RHIRasterState &other) const {
        return mode == other.mode &&
               cullMode == other.cullMode &&
               frontFace == other.frontFace &&
               lineWidth == other.lineWidth;
    }
};

/**
 * @class RHIDepthStencilState
 * @brief RHI depth stencil state descriptor
 */
struct RHIDepthStencilState {
    RHICompareFunction depthCompare : 4;
    bool depthEnable : 1;
    bool depthWrite : 1;
    bool stencilEnable : 1;
    uint8 writeMask;
    uint8 referenceValue;
    uint8 compareMask;
    RHIOperation sfail : 4;
    RHIOperation dfail : 4;
    RHIOperation dpass : 4;
    RHICompareFunction compareFunction : 4;

    RHIDepthStencilState() {
        Memory::Set(this, 0x0, sizeof(RHIDepthStencilState));
        depthCompare = RHICompareFunction::Always;
        depthEnable = false;
        depthWrite = false;
        stencilEnable = false;
        writeMask = 0;
        referenceValue = 0;
        compareMask = 0;
        sfail = RHIOperation::Zero;
        dfail = RHIOperation::Zero;
        dpass = RHIOperation::Zero;
        compareFunction = RHICompareFunction::Always;
    }

    bool operator==(const RHIDepthStencilState &other) const {
        return depthCompare == other.depthCompare &&
               depthEnable == other.depthEnable &&
               depthWrite == other.depthWrite &&
               stencilEnable == other.stencilEnable &&
               writeMask == other.writeMask &&
               referenceValue == other.referenceValue &&
               compareMask == other.compareMask &&
               sfail == other.sfail &&
               dfail == other.dfail &&
               dpass == other.dpass &&
               compareFunction == other.compareFunction;
    }

    static RHIDepthStencilState CreateDepthState(bool depthWrite = true) {
        RHIDepthStencilState dss;

        dss.depthCompare = RHICompareFunction::Less;
        dss.depthEnable = true;
        dss.depthWrite = depthWrite;

        return dss;
    }
};

/**
 * @class RHIBlendAttachment
 * @brief RHI single blend attachment descriptor
 */
struct RHIBlendAttachment {
    bool enable : 1;
    RHIBlendOperation alphaBlendOp : 3;
    RHIBlendOperation colorBlendOp : 3;
    RHIBlendFactor srcAlphaBlendFactor : 4;
    RHIBlendFactor srcColorBlendFactor : 4;
    RHIBlendFactor dstAlphaBlendFactor : 4;
    RHIBlendFactor dstColorBlendFactor : 4;

    RHIBlendAttachment() {
        Memory::Set(this, 0x0, sizeof(RHIBlendAttachment));
        enable = false;
        alphaBlendOp = RHIBlendOperation::Add;
        colorBlendOp = RHIBlendOperation::Add;
        srcAlphaBlendFactor = RHIBlendFactor::Zero;
        srcColorBlendFactor = RHIBlendFactor::Zero;
        dstAlphaBlendFactor = RHIBlendFactor::Zero;
        dstColorBlendFactor = RHIBlendFactor::Zero;
    }

    bool operator==(const RHIBlendAttachment &other) const {
        return (enable == false && other.enable == false) ||
               (enable == other.enable &&
                alphaBlendOp == other.alphaBlendOp &&
                colorBlendOp == other.colorBlendOp &&
                srcAlphaBlendFactor == other.srcAlphaBlendFactor &&
                srcColorBlendFactor == other.srcColorBlendFactor &&
                dstAlphaBlendFactor == other.dstAlphaBlendFactor &&
                dstColorBlendFactor == other.dstColorBlendFactor);
    }
};

/**
 * @class RHIBlendState
 * @brief RHI blend state descriptor
 */
struct RHIBlendState {
    std::vector<RHIBlendAttachment> attachments;

    bool operator==(const RHIBlendState &other) const {
        if (attachments.size() != other.attachments.size())
            return false;

        for (size_t i = 0; i < attachments.size(); i++) {
            if (!(attachments[i] == other.attachments[i]))
                return false;
        }

        return true;
    }

    static RHIBlendState CreateBlendState(uint32 attachments) {
        RHIBlendState bs;
        bs.attachments.resize(attachments);

        for (uint32 i = 0; i < attachments; i++) {
            auto &attachment = bs.attachments[i];
            attachment.enable = true;
            attachment.alphaBlendOp = RHIBlendOperation::Add;
            attachment.colorBlendOp = RHIBlendOperation::Add;
            attachment.srcAlphaBlendFactor = RHIBlendFactor::SrcAlpha;
            attachment.srcColorBlendFactor = RHIBlendFactor::SrcAlpha;
            attachment.dstAlphaBlendFactor = RHIBlendFactor::OneMinusSrcAlpha;
            attachment.dstColorBlendFactor = RHIBlendFactor::OneMinusSrcAlpha;
        }

        return bs;
    }
};

/**
 * @class RHIGraphicsPipelineDesc
 * @brief Graphics pipeline descriptor
 */
class RHIGraphicsPipelineDesc {
public:
    RHIDepthStencilState depthStencilState;
    RHIRasterState rasterState;
    RHIBlendState blendState;
    RHIPrimitivesType primitivesType;
    Ref<RHIShader> shader;
    Ref<RHIVertexDeclaration> declaration;
    Ref<RHIRenderPass> renderPass;
};

/**
 * @class RHIGraphicsPipeline
 * @brief Graphics pipeline
 */
class RHIGraphicsPipeline : public RHIResource {
public:
    BRK_API ~RHIGraphicsPipeline() override = default;

    /** @return Pipeline descriptor */
    const RHIGraphicsPipelineDesc &GetDesc() const { return mDesc; }

protected:
    /** Pipeline descriptor */
    RHIGraphicsPipelineDesc mDesc;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RHIGRAPHICSPIPELINE_HPP
