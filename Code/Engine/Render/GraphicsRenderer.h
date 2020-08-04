/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GRAPHICSRENDERER_H
#define BERSERK_GRAPHICSRENDERER_H

#include <Graphics.h>
#include <GraphicsTexturesRenderer.h>

namespace Berserk {
    namespace Render {

        /** Encapsulates logic, required for rendering graphics area on GPU */
        class GraphicsRenderer {
        public:

            /**
             * Construct renderer for provided graphics instance
             * @param graphics Graphics area to be drawn
             */
            GraphicsRenderer(Graphics& graphics);

            /** Clear cache if graphics was updated */
            void clearState();

            /** Draw graphics state with specified RHI draw list */
            void draw(RHIDrawList& drawList);

        private:

            /** Init rendering pipeline for drawing */
            void initialize();

            /** Prepare data before rendering */
            void prepareData();

            /** Graphics area to be drawn */
            Graphics* mGraphics;

            /** Encapsulates textures rendering */
            GraphicsTexturesRenderer mTexturesRenderer;

        };

    }
}

#endif //BERSERK_GRAPHICSRENDERER_H