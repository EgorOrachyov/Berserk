/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIGRAPHICSPIPELINE_H
#define BERSERK_RHIGRAPHICSPIPELINE_H

#include <RHI/RHIResource.h>

namespace Berserk {

    class RHIGraphicsPipeline : public RHIResource {
    public:
        ~RHIGraphicsPipeline() override = default;

        /** @return Pipeline shader program */
        TPtrShared<RHIShader> getShader() const { return mShader; }

        /** @return Primitives rendered through the pipeline */
        EPrimitivesType getPrimitivesType() const { return mPrimitivesType; }

        /** @return Mode of rasterized primitives on the screen */
        EPolygonMode getPolygonMode() const { return mPolygonMode; }

        /** @return Culling mode */
        EPolygonCullMode getPolygonCullMode() const { return mPolygonCullMode; }

        /** @return From facing to determine visible faces of primitives */
        EPolygonFrontFace getPolygonFrontFace() const { return mPolygonFrontFace; }

        /** @return Line with */
        float getLineWidth() const { return mLineWidth; }

        /** @return True if pipeline uses depth test */
        bool getDepthTest() const { return mDepthTest; }

        /** @return True if pipeline uses writes depth values to the depth buffer */
        bool getDepthWrite() const { return mDepthWrite; }

        /** @return Depth compare function between new value and value in the buffer */
        ECompareFunction getDepthCompareFunction() const { return mDepthCompare; }

        /** @return Blend state descriptor */
        const RHIBlendStateDesc &getBlendStateDesc() const { return mBlendStateDesc; }

        /** @return Stencil state description */
        const RHIStencilStateDesc &getStencilStateDesc() const { return mStencilStateDesc; }

        /** @return Pipeline framebuffer format descriptor */
        const RHIFramebufferFormatDesc &getFramebufferDesc() const { return mFramebufferDesc; }

    protected:
        /** Pipeline shader program */
        TPtrShared<RHIShader> mShader;

        /** Primitives rendered through the pipeline */
        EPrimitivesType mPrimitivesType;

        /** Mode of rasterized primitives on the screen */
        EPolygonMode mPolygonMode;

        /** Culling mode */
        EPolygonCullMode mPolygonCullMode;

        /** From facing to determine visible faces of primitives */
        EPolygonFrontFace mPolygonFrontFace;

        /** Line with */
        float mLineWidth;

        /** True if pipeline uses depth test */
        bool mDepthTest;

        /** True if pipeline uses writes depth values to the depth buffer */
        bool mDepthWrite;

        /** Depth compare function between new value and value in the buffer */
        ECompareFunction mDepthCompare;

        /** Blend state descriptor */
        RHIBlendStateDesc mBlendStateDesc; // todo: support blending

        /** Stencil state description */
        RHIStencilStateDesc mStencilStateDesc; // todo: support stencil test

        /** Pipeline framebuffer format descriptor */
        RHIFramebufferFormatDesc mFramebufferDesc;
    };

}

#endif //BERSERK_RHIGRAPHICSPIPELINE_H
