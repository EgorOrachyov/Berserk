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

#ifndef BERSERK_RHIPIPELINESTATE_HPP
#define BERSERK_RHIPIPELINESTATE_HPP

#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkRHI/RHIProgram.hpp>
#include <BerserkRHI/RHIVertexDeclaration.hpp>

namespace Berserk {
    namespace RHI {

        class PipelineState {
        public:

            struct RasterState {
                PolygonMode mode: 2;
                PolygonCullMode cullMode: 3;
                PolygonFrontFace frontFace: 3;
                uint8 lineWidth;

                RasterState() {
                    Memory::Set(this, 0x0, sizeof(RasterState));
                    mode = PolygonMode::Fill;
                    cullMode = PolygonCullMode::Disabled;
                    frontFace = PolygonFrontFace::CounterClockwise;
                    lineWidth = 1;
                }

                bool operator==(const RasterState& other) const {
                    return mode == other.mode &&
                           cullMode == other.cullMode &&
                           frontFace == other.frontFace &&
                           lineWidth == other.lineWidth;
                }
            };

            struct DepthStencilState {
                CompareFunction depthCompare: 4;
                bool depthEnable: 1;
                bool depthWrite: 1;
                bool stencilEnable: 1;
                uint8 writeMask;
                uint8 referenceValue;
                uint8 compareMask;
                Operation sfail: 4;
                Operation dfail: 4;
                Operation dpass: 4;
                CompareFunction compareFunction : 4;

                DepthStencilState() {
                    Memory::Set(this, 0x0, sizeof(DepthStencilState));
                    depthCompare = CompareFunction::Always;
                    depthEnable = false;
                    depthWrite = false;
                    stencilEnable = false;
                    writeMask = 0;
                    referenceValue = 0;
                    compareMask = 0;
                    sfail = Operation::Zero;
                    dfail = Operation::Zero;
                    dpass = Operation::Zero;
                    compareFunction = CompareFunction::Always;
                }

                bool operator==(const DepthStencilState& other) const {
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

                static DepthStencilState CreateDepthState(bool depthWrite = true) {
                    DepthStencilState dss;

                    dss.depthCompare = CompareFunction::Less;
                    dss.depthEnable = true;
                    dss.depthWrite = depthWrite;

                    return dss;
                }
            };

            struct BlendAttachment {
                bool enable : 1;
                BlendOperation alphaBlendOp: 3;
                BlendOperation colorBlendOp: 3;
                BlendFactor srcAlphaBlendFactor: 4;
                BlendFactor srcColorBlendFactor: 4;
                BlendFactor dstAlphaBlendFactor: 4;
                BlendFactor dstColorBlendFactor: 4;

                BlendAttachment() {
                    Memory::Set(this, 0x0, sizeof(BlendAttachment));
                    enable = false;
                    alphaBlendOp = BlendOperation::Add;
                    colorBlendOp = BlendOperation::Add;
                    srcAlphaBlendFactor = BlendFactor::Zero;
                    srcColorBlendFactor = BlendFactor::Zero;
                    dstAlphaBlendFactor = BlendFactor::Zero;
                    dstColorBlendFactor = BlendFactor::Zero;
                }

                bool operator==(const BlendAttachment& other) const {
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

            struct BlendState {
                ArrayFixed<BlendAttachment, Limits::MAX_COLOR_ATTACHMENTS> attachments;

                bool operator==(const BlendState& other) const {
                    if (attachments.GetSize() != other.attachments.GetSize())
                        return false;

                    for (uint32 i = 0; i < attachments.GetSize(); i++) {
                        if (!(attachments[i] == other.attachments[i]))
                            return false;
                    }

                    return true;
                }

                static BlendState CreateBlendState(uint32 attachments) {
                    BlendState bs;
                    bs.attachments.Resize(attachments);

                    for (uint32 i = 0; i < attachments; i++) {
                        auto& attachment = bs.attachments[i];
                        attachment.enable = true;
                        attachment.alphaBlendOp = BlendOperation::Add;
                        attachment.colorBlendOp = BlendOperation::Add;
                        attachment.srcAlphaBlendFactor = BlendFactor::SrcAlpha;
                        attachment.srcColorBlendFactor = BlendFactor::SrcAlpha;
                        attachment.dstAlphaBlendFactor = BlendFactor::OneMinusSrcAlpha;
                        attachment.dstColorBlendFactor = BlendFactor::OneMinusSrcAlpha;
                    }

                    return bs;
                }
            };

            DepthStencilState depthStencilState;
            RasterState rasterState;
            BlendState blendState;
            RcPtr<Program> program;
            RcPtr<VertexDeclaration> declaration;
            PrimitivesType primitivesType;
        };

    }
}

#endif //BERSERK_RHIPIPELINESTATE_HPP