/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_BATCHEDELEMENTSRENDERER_H
#define BERSERK_BATCHEDELEMENTSRENDERER_H

#include <BatchedElements.h>
#include <ShaderCore/Shader.h>
#include <RenderResources/VertexArray.h>
#include <ViewData.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Bathed elements renderer
         *
         * Accepts batched elements container, view settings, allocates required GPU
         * resources, such as shaders and buffers, and performs rendering of the elements.
         */
        class BatchedElementsRenderer {
        public:

            static const uint32 INITIAL_LINES_COUNT_PER_BATCH = 10;

            /** Initializes render internal pipeline and buffers */
            BatchedElementsRenderer();

            void draw(const ViewData& viewData, const BatchedElements& batch, RHIDrawList& drawList);

        private:

            /** Initializes render internal pipeline and buffers */
            void initializePipeline();

            /** Packs data */
            void prepareData(const BatchedElements& elements);

            uint32 mMaxLines;
            uint32 mMaxPoints;
            uint32 mMaxBoxes;
            uint32 mMaxSpheres;

            TPtrShared<VertexArray> mLinesArrayData;
            TPtrShared<VertexArray> mPointsArrayData;
            TPtrShared<VertexArray> mBoxesArrayData;
            TPtrShared<VertexArray> mSpheresArrayData;

            TPtrShared<Shader> mLinesShader;
            TPtrShared<Shader> mPointsShader;
            TPtrShared<Shader> mBoxesShader;
            TPtrShared<Shader> mSpheresShader;

            TPtrShared<ShaderBindings> mLinesBindings;
            TPtrShared<ShaderBindings> mPointsBindings;
            TPtrShared<ShaderBindings> mBoxesBindings;
            TPtrShared<ShaderBindings> mSpheresBindings;
        };

    }
}

#endif //BERSERK_BATCHEDELEMENTSRENDERER_H