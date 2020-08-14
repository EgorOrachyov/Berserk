/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SCENEVIEW_H
#define BERSERK_SCENEVIEW_H

#include <Graphics.h>
#include <Scene/ViewData.h>
#include <TPtrShared.h>
#include <Scene/Scene.h>
#include <Scene/Camera.h>
#include <RenderTargets/RenderTarget.h>
#include <BatchedElementsRenderer.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Scene view
         *
         * Single view of the scene rendered to some render target with
         * specified camera, viewport setting and optional on-screen graphics.
         */
        class SceneView {
        public:

            /** This view */
            ViewData viewData;

            /** Final target to present an image */
            TPtrShared<RenderTarget> target;

            /** View camera of the scene */
            TPtrShared<Camera> camera;

            /** Actual scene to be drawn */
            TPtrShared<Scene> scene;

            /** 2D graphics area, shown on top of the view */
            TPtrShared<Graphics> graphics;


            // todo: move to scene view renderer
            TPtrUnique<BatchedElementsRenderer> batchRenderer;

        };
    }
}

#endif //BERSERK_SCENEVIEW_H
