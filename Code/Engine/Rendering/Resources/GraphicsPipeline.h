/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GRAPHICSPIPELINE_H
#define BERSERK_GRAPHICSPIPELINE_H

#include <Rendering/Resources/IRenderResource.h>
#include <RHI/RHIResources.h>


namespace Berserk {
    namespace Rendering {

        class GraphicsPipeline : public IRenderResource {
        public:
            /**
             * Create pipeline with basic setup (no stencil test and alpha blend)
             * @param shader Compiled shader module
             * @param depthTest If must check depth
             * @param depthWrite If must write depth
             * @param primitivesType Type of assembled primitives
             * @param cullMode Culling mode
             * @param polygonMode Fill mode of polygons
             * @param target Target rendering surface (to extract format)
             */
            GraphicsPipeline(const TPtrShared<RHIShader> &shader, bool depthTest, bool depthWrite,
                             EPrimitivesType primitivesType, EPolygonCullMode cullMode, EPolygonMode polygonMode,
                             const class IRenderTarget &target);

            GraphicsPipeline() = default;
            ~GraphicsPipeline() override;

            bool isInitialized() const override;
            bool isInitializedRHI() const override;
            CString getFriendlyName() const override;

            const TPtrShared<RHIGraphicsPipeline> &getPipelineRHI() const { return mPipeline; };

        protected:
            TPtrShared<RHIGraphicsPipeline> mPipeline;
        };

    }
}

#endif //BERSERK_GRAPHICSPIPELINE_H