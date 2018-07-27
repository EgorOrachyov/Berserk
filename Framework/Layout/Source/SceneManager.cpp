//
// Created by Egor Orachyov on 27.07.2018.
//

#include "Managers/SceneManager.h"

namespace Berserk
{

    SceneManager::SceneManager()
    {

    }

    SceneManager::~SceneManager()
    {

    }

    ObjectManager &SceneManager::getObjectManager()
    {
        return mObjectManager;
    }

    RenderManager &SceneManager::getRenderManager()
    {
        return mRenderManager;
    }

    SceneManager &SceneManager::getInstanceRef()
    {
        return *gSceneManager;
    }

    SceneManager *SceneManager::getInstancePtr()
    {
        return gSceneManager;
    }

    SceneManager *gSceneManager;

} // namespace Berserk