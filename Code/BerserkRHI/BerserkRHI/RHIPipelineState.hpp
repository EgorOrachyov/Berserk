/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
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
                    depthCompare = CompareFunction::Less;
                    depthEnable = false;
                    depthWrite = false;
                    stencilEnable = false;
                    writeMask = 0;
                    referenceValue = 0;
                    compareMask = 0;
                    sfail = Operation::Keep;
                    dfail = Operation::Keep;
                    dpass = Operation::Keep;
                    compareFunction = CompareFunction::Never;
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
                    srcAlphaBlendFactor = BlendFactor::One;
                    srcColorBlendFactor = BlendFactor::One;
                    dstAlphaBlendFactor = BlendFactor::One;
                    dstColorBlendFactor = BlendFactor::One;
                }
            };

            struct BlendState {
                ArrayFixed<BlendAttachment, Limits::MAX_COLOR_ATTACHMENTS> attachments;
            };

            DepthStencilState depthStencilState;
            RasterState rasterState;
            BlendState blendState;
            Ref<Program> program;
            Ref<VertexDeclaration> declaration;
        };

    }
}

#endif //BERSERK_RHIPIPELINESTATE_HPP