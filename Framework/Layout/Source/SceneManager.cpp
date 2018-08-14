//
// Created by Egor Orachyov on 27.07.2018.
//

#include "Managers/SceneManager.h"
#include "Render/RenderSystem.h"

namespace Berserk
{

    SceneManager::SceneManager()
    {
        mShouldErrorClose = false;
        mShouldChangeScene = false;

        mObjectManager.init();

        mCurrentScene = NULL;
        mNextScene = NULL;
    }

    SceneManager::~SceneManager()
    {

    }

    void SceneManager::init()
    {
        if (mNextScene == NULL || mShouldChangeScene == false)
        {
            ERROR("No scene to make active. Call addScene to add active scene");
            return;
        }
        if (mShouldErrorClose)
        {
            ERROR("Ask for error close of scene manager");
            return;
        }

        mCurrentScene = mNextScene;
        mNextScene = NULL;

        mCurrentScene->onInit();
        mCurrentScene->init();

        mShouldChangeScene = false;
        mShouldErrorClose = false;

        mTimer.init();
        mTimer.start();
    }

    void SceneManager::update()
    {
        if (!mCurrentScene)
        {
            ERROR("SceneManager: Have no scene to process");
            return;
        }
        if (mShouldErrorClose)
        {
            ERROR("Ask for error close of scene manager");
            mCurrentScene->onDestroy();
            mCurrentScene->destroy();
            mCurrentScene = NULL;
            return;
        }

        mTimer.stop();
        FLOAT64 delta = mTimer.getSecondsInInterval();
        mTimer.start();

        mCurrentScene->onUpdate(delta);

        mCurrentScene->process(SceneInfo::SI_LAYOUT_0, delta);
        gRenderSystem->renderPass1();

        mCurrentScene->process(SceneInfo::SI_LAYOUT_1, delta);
        gRenderSystem->renderPass2();

        if (mShouldChangeScene && mNextScene != NULL)
        {
            mCurrentScene->onDestroy();
            mCurrentScene->destroy();

            mNextScene->onInit();
            mNextScene->init();

            mCurrentScene = mNextScene;
            mNextScene = NULL;
        }

        mShouldChangeScene = false;
        mShouldErrorClose = false;
    }

    void SceneManager::destroy()
    {
        if (!mCurrentScene)
        {
            ERROR("SceneManager: Have no scene to process");
            return;
        }

        mCurrentScene->onDestroy();
        mCurrentScene->destroy();
        mCurrentScene = NULL;

        mShouldChangeScene = false;
        mShouldErrorClose = false;
    }

    void SceneManager::addObject(Object *object)
    {
        mObjectManager.add(object);
    }

    void SceneManager::addScene(Scene *scene, bool makeCurrent)
    {
        mObjectManager.add(scene);

        if (makeCurrent)
        {
            mNextScene = scene;
            mShouldChangeScene = true;
        }
    }

    Scene* SceneManager::getCurrentScene()
    {
        return mCurrentScene;
    }

    Scene* SceneManager::getNextScene()
    {
        return mNextScene;
    }

    Object* SceneManager::getObject(const CStaticString &name) const
    {
        return mObjectManager.get(name);
    }

    Actor* SceneManager::getActor(const CStaticString &name) const
    {
        return dynamic_cast<Actor*>(mObjectManager.get(name));
    }

    Camera* SceneManager::getCamera(const CStaticString &name) const
    {
        return dynamic_cast<Camera*>(mObjectManager.get(name));
    }

    Scene* SceneManager::getScene(const CStaticString &name) const
    {
        return dynamic_cast<Scene*>(mObjectManager.get(name));
    }

    ObjectManager &SceneManager::getObjectManager()
    {
        return mObjectManager;
    }

    void SceneManager::askErrorClose(bool close)
    {
        mShouldErrorClose = close;
    }

    void SceneManager::askSceneChanging(const CStaticString &name)
    {
        auto *scene = dynamic_cast<Scene*>(mObjectManager.get(name));

        if (scene == NULL)
        {
            WARNING("Cannot find scene %s to change active scene", name.getChars());
            return;
        }

        mShouldChangeScene = true;
        mNextScene = scene;
    }

    bool SceneManager::isErrorClose() const
    {
        return mShouldErrorClose;
    }

    bool SceneManager::isSceneChanging() const
    {
        return mShouldChangeScene;
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