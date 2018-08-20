//
// Created by Egor Orachyov on 27.07.2018.
//

#ifndef BERSERKENGINE_SCENEMANAGER_H
#define BERSERKENGINE_SCENEMANAGER_H

#include "Managers/ObjectManager.h"

#include "Objects/Object.h"
#include "Objects/Actor.h"
#include "Objects/Cameras/Camera.h"
#include "Objects/Scene/Scene.h"

#include "Essential/UsageDescriptors.h"
#include "Containers/LinkedList.h"
#include "Timer/GLTimer.h"

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

        void setRenderCamera(Camera* camera) const;
        Camera* getRenderCamera() const;

        void addObject(Object *object);
        void addScene(Scene *scene, bool makeCurrent = false);

        Scene *getCurrentScene();
        Scene *getNextScene();

        Object *getObject(const CStaticString &name) const;
        Actor  *getActor(const CStaticString &name) const;
        Camera *getCamera(const CStaticString &name) const;
        Scene  *getScene(const CStaticString &name) const;

        ObjectManager &getObjectManager();

        void askErrorClose(bool close = true);
        void askSceneChanging(const CStaticString &name);

        bool isErrorClose() const;
        bool isSceneChanging() const;

        static SceneManager &getInstanceRef();
        static SceneManager *getInstancePtr();

    private:

        INT8 mShouldErrorClose  : 1;
        INT8 mShouldChangeScene : 1;

        Scene *mCurrentScene;
        Scene *mNextScene;

        ObjectManager mObjectManager;

        /// debug ///

        GLTimer mTimer;
    };

    /// Should be initialized via Application Context
    extern SceneManager *gSceneManager;

} // namespace Berserk

#endif //BERSERKENGINE_SCENEMANAGER_H