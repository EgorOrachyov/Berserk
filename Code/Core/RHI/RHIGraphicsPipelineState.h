/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIGRAPHICSPIPELINESTATE_H
#define BERSERK_RHIGRAPHICSPIPELINESTATE_H

#include <RHI/RHIResource.h>
#include <RHI/RHIShader.h>

namespace Berserk {

    class RHIGraphicsPipelineState {
    public:

        RHIGraphicsPipelineState() {
            polygonMode = EPolygonMode::Fill;
            polygonCullMode = EPolygonCullMode::Disabled;
            polygonFrontFace = EPolygonFrontFace::CounterClockwise;
            lineWidth = 1.0f;
            depthTest = false;
            depthWrite = false;
            depthCompare = ECompareFunction::Less;
        }

        /** Mode of primitives rasterization on the screen */
        EPolygonMode polygonMode : 2;

        /** Culling mode */
        EPolygonCullMode polygonCullMode : 3;

        /** From facing to determine visible faces of primitives */
        EPolygonFrontFace polygonFrontFace : 3;

        /** Depth compare function between new value and value in the buffer */
        ECompareFunction depthCompare : 4;

        /** True if pipeline uses depth test */
        bool depthTest : 1;

        /** True if pipeline uses writes depth values to the depth buffer */
        bool depthWrite : 1;

        /** Line with */
        float lineWidth = 1.0f;

        /** Pipeline shader program */
        TPtrShared<class RHIShader> shader;

        /** Layout of the input vertices */
        TPtrShared<class RHIVertexDeclaration> vertexDeclaration;

        /** Blend state descriptor */
        RHIBlendStateDesc blendState;

        /** Stencil state description */
        RHIStencilStateDesc stencilState;
    };

}

#endif //BERSERK_RHIGRAPHICSPIPELINESTATE_H
