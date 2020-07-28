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

namespace Berserk {

    class RHIGraphicsPipelineState {
    public:

        /** Pipeline shader program */
        TPtrShared<class RHIShader> shader;

        /** Layout of the input vertices */
        TPtrShared<class RHIVertexDeclaration> vertexDeclaration;

        /** Mode of primitives rasterization on the screen */
        EPolygonMode polygonMode = EPolygonMode::Fill;

        /** Culling mode */
        EPolygonCullMode polygonCullMode = EPolygonCullMode::Back;

        /** From facing to determine visible faces of primitives */
        EPolygonFrontFace polygonFrontFace = EPolygonFrontFace::CounterClockwise;

        /** Line with */
        float lineWidth = 1.0f;

        /** True if pipeline uses depth test */
        bool depthTest = true;

        /** True if pipeline uses writes depth values to the depth buffer */
        bool depthWrite = false;

        /** Depth compare function between new value and value in the buffer */
        ECompareFunction depthCompare = ECompareFunction::Less;

        /** Blend state descriptor */
        RHIBlendStateDesc blendState;

        /** Stencil state description */
        RHIStencilStateDesc stencilState;
    };

}

#endif //BERSERK_RHIGRAPHICSPIPELINESTATE_H
