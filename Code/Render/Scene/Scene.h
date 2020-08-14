/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SCENE_H
#define BERSERK_SCENE_H

#include <TPtrShared.h>
#include <BatchedElements.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief 3D scene
         *
         * Scene of  visible objects in the 3-dimensional space.
         * The same scene could be rendered to the multiple views with different view settings.
         */
        class Scene {
        public:

            /** Scene batched simple elements for debug */
            TPtrShared<BatchedElements> mBatch;

            /** Scene batched elements rendered on top of the other objects debug */
            TPtrShared<BatchedElements> mBatchOverlay;

        };

    }
}

#endif //BERSERK_SCENE_H