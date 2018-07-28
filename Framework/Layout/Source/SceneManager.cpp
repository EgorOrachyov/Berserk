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

    Object& SceneManager::getObject(const CStaticString &name) const
    {
        return *mObjectManager.get(name);
    }

    Actor& SceneManager::getActor(const CStaticString &name) const
    {
        return *(Actor*)mObjectManager.get(name);
    }

    Camera& SceneManager::getCamera(const CStaticString &name) const
    {
        return *(Camera*)mObjectManager.get(name);
    }

    ObjectManager &SceneManager::getObjectManager()
    {
        return mObjectManager;
    }

    RenderManager &SceneManager::getRenderManager()
    {
        return mRenderManager;
    }

    void SceneManager::askForErrorClose(bool close)
    {
        mShouldErrorClose = close;
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