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

            BatchedElementsRenderer(const BatchedElements& batchedElements);
            ~BatchedElementsRenderer();

        private:

            TPtrShared<VertexArray> mLinesArrayData;
            TPtrShared<VertexArray> mPointsArrayData;
            TPtrShared<VertexArray> mBoxesArrayData;
            TPtrShared<VertexArray> mSpheresArrayData;


        };

    }
}

#endif //BERSERK_BATCHEDELEMENTSRENDERER_H