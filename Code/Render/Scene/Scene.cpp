/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Scene/Scene.h>

namespace Berserk {
    namespace Render {

        void Scene::addRenderObject(Berserk::TPtrShared<RenderObject> renderObject) {
            BERSERK_COND_ERROR_RET(renderObject.isNotNull(), "Provided null object");
            BERSERK_COND_ERROR_RET(!renderObject->isAttachedToScene(), "Object already attached to the scene");

            uint32 index = mRenderObjects.size();

            mRenderObjects.add(renderObject);

            renderObject->attachToScene(*this, index);
        }

        void Scene::removeRenderObject(Berserk::TPtrShared<RenderObject> renderObject) {
            BERSERK_COND_ERROR_RET(renderObject.isNotNull(), "Provided null object");
            BERSERK_COND_ERROR_RET(renderObject->isAttachedToScene(*this), "Object was not attached to this scene");

            uint32 index = renderObject->getSceneIndex();

            if (mRenderObjects.size() == index + 1) {
                mRenderObjects.removeUnordered(index);
            }
            else {
                uint32 last = mRenderObjects.size() - 1;

                auto moved = mRenderObjects[last];
                moved->updateSceneIndex(index);
                mRenderObjects[index] = moved;

                mRenderObjects.removeUnordered(index);
            }

            renderObject->detachFromScene();
        }

    }
}