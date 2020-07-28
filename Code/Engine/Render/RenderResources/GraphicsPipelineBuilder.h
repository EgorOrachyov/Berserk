/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GRAPHICSPIPELINEBUILDER_H
#define BERSERK_GRAPHICSPIPELINEBUILDER_H

#include <RHI/RHIDescs.h>
#include <RHI/RHIGraphicsPipelineState.h>

namespace Berserk {
    namespace Render {

        /** Utility for creating graphics pipelines */
        class GraphicsPipelineBuilder {
        public:

            using Builder = GraphicsPipelineBuilder;

            GraphicsPipelineBuilder() = default;
            ~GraphicsPipelineBuilder() = default;

            /** Specify shader program stages */
            Builder& setShader(const TPtrShared<class ShaderProgram> &shader);

            /** Specify vertex declaration */
            Builder& setDeclaration(const TPtrShared<class VertexDeclaration> &declaration);

            /** How to fill/draw polygons */
            Builder& polygonMode(EPolygonMode polygonMode);

            /** How cull polygons */
            Builder& polygonCullMode(EPolygonCullMode cullMode);

            /** How define front of the polygons */
            Builder& polygonFrontFace(EPolygonFrontFace frontFace);

            /** Rasterization line width */
            Builder& lineWidth(float lineWidth);

            /** Enable depth test (or disable if passed false) */
            Builder& depthTest(bool enable);

            /** Enable depth value write */
            Builder& depthWrite(bool enable);

            /** Specify depth compare function */
            Builder& depthFunction(ECompareFunction compareFunction);

            /** Enable stencil test (or disable if passed false) */
            Builder& stencilTest(bool enable);

            /** Specify stencil mask to write values in buffer */
            Builder& stencilWriteMask(uint32 mask);

            /** Specify compare options for stencil test */
            Builder& stencilCompare(ECompareFunction compareFunction, uint32 reference, uint32 mask);

            /** Specify operations for stencil test results */
            Builder& stencilOp(EOperation sfail, EOperation dfail, EOperation dpass);

            /** Enable blend operation (or disable if passed false) */
            Builder& blend(bool enable);

            /** @return Instance of the graphics pipeline (null if failed to create) */
            RHIGraphicsPipelineState build();

        private:

            friend class GraphicsPipeline;

            RHIGraphicsPipelineState mPipelineState;
            TPtrShared<class ShaderProgram> mShaderProgram;
            TPtrShared<class VertexDeclaration> mVertexDeclaration;

        };

    }
}

#endif //BERSERK_GRAPHICSPIPELINEBUILDER_H
