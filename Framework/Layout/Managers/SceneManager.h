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

namespace Berserk
{
    class SceneManager
    {
    public:

        SceneManager();
        ~SceneManager();

        Object &getObject(const CStaticString &name) const;
        Actor  &getActor(const CStaticString &name) const;
        Camera &getCamera(const CStaticString &name) const;

        ObjectManager &getObjectManager();
        RenderManager &getRenderManager();

        static SceneManager &getInstanceRef();
        static SceneManager *getInstancePtr();

    private:

        ObjectManager mObjectManager;
        RenderManager mRenderManager;

    };

    /// Should be initialized via Application Context
    extern SceneManager *gSceneManager;

} // namespace Berserk

#endif //BERSERKENGINE_SCENEMANAGER_H