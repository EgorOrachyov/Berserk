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
                    mode = PolygonMode::Fill;
                    cullMode = PolygonCullMode::Disabled;
                    frontFace = PolygonFrontFace::CounterClockwise;
                    lineWidth = 1;
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
                    enable = false;
                    alphaBlendOp = BlendOperation::Add;
                    colorBlendOp = BlendOperation::Add;
                    srcAlphaBlendFactor = BlendFactor::Zero;
                    srcColorBlendFactor = BlendFactor::Zero;
                    dstAlphaBlendFactor = BlendFactor::Zero;
                    dstColorBlendFactor = BlendFactor::Zero;
                }
            };

            struct BlendState {
                ArrayFixed<BlendAttachment, Limits::MAX_COLOR_ATTACHMENTS> attachments;

                static BlendState CreateBlendState(size_t attachments) {
                    BlendState bs;

                    for (size_t i = 0; i < attachments; i++) {
                        BlendAttachment attachment{};
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
            RefCounted<Program> program;
            RefCounted<VertexDeclaration> declaration;
        };

    }
}

#endif //BERSERK_RHIPIPELINESTATE_HPP