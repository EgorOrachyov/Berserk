/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GRAPHICSPIPELINE_H
#define BERSERK_GRAPHICSPIPELINE_H

#include <ShaderCore/ShaderProgram.h>
#include <RenderResources/RenderResource.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Graphics pipeline
         *
         * Represents complete graphics pipeline state with RHI resources,
         * required for full rendering configuration process.
         */
        class GraphicsPipeline : public RenderResource {
        public:

            GraphicsPipeline(const class GraphicsPipelineBuilder& builder);
            ~GraphicsPipeline() override = default;

            bool isInitialized() const override;
            bool isInitializedRHI() const override;
            CString getFriendlyName() const override;

            /** @return Pipeline name for look-up (the same as shader program has) */
            const CString& getPipelineName() const { return mPipelineName; }

            /** @return Shader program used to create this pipeline */
            const TPtrShared<ShaderProgram>& getShaderProgram() const { return mShader; }

            /** @return This pipeline RHI resource handle */
            const TPtrShared<RHIGraphicsPipeline>& getRHI() const { return mPipelineRHI; }

        private:

            CString mPipelineName;
            TPtrShared<ShaderProgram> mShader;
            TPtrShared<RHIGraphicsPipeline> mPipelineRHI;

        };

    }
}




#endif //BERSERK_GRAPHICSPIPELINE_H
