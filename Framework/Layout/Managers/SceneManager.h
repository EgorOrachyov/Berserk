//
// Created by Egor Orachyov on 27.07.2018.
//

#ifndef BERSERKENGINE_SCENEMANAGER_H
#define BERSERKENGINE_SCENEMANAGER_H

#include "Managers/RenderManager.h"
#include "Managers/ObjectManager.h"

#include "Objects/Object.h"
#include "Objects/Actor.h"
#include "Objects/Cameras/Camera.h"
#include "Objects/Scene/Scene.h"

#include "Essential/UsageDescriptors.h"
#include "Containers/LinkedList.h"

namespace Berserk
{
    class ENGINE_EXPORT SceneManager
    {
    public:

        SceneManager();
        virtual ~SceneManager();

        void init();
        void update();
        void destroy();

        void addScene(Scene *scene);
        Scene &getCurrentScene();

        Object &getObject(const CStaticString &name) const;
        Actor &getActor(const CStaticString &name) const;
        Camera &getCamera(const CStaticString &name) const;
        Scene &getScene(const CStaticString &name) const;

        ObjectManager &getObjectManager();
        RenderManager &getRenderManager();

        void askForErrorClose(bool close = true);
        void askForSceneChanging(const CStaticString &name);

        static SceneManager &getInstanceRef();
        static SceneManager *getInstancePtr();

    private:

        INT8 mShouldErrorClose : 1;
        INT8 mShouldChangeScene : 1;

        Scene *mCurrentScene;
        Scene *nextScene;
        LinkedList<Scene*> mScenePool;

        ObjectManager mObjectManager;
        RenderManager mRenderManager;

    };

    /// Should be initialized via Application Context
    extern SceneManager *gSceneManager;

} // namespace Berserk

#endif //BERSERKENGINE_SCENEMANAGER_H