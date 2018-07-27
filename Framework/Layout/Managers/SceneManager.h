//
// Created by Egor Orachyov on 27.07.2018.
//

#ifndef BERSERKENGINE_SCENEMANAGER_H
#define BERSERKENGINE_SCENEMANAGER_H

#include "Managers/RenderManager.h"
#include "Managers/ObjectManager.h"

namespace Berserk
{
    class SceneManager
    {
    public:

        SceneManager();
        ~SceneManager();

        ObjectManager &getObjectManager() const;
        RenderManager &getRenderManager() const;

        static SceneManager &getInstanceRef();
        static SceneManager *getInstancePtr();

    private:

        ObjectManager mObjectManager;
        RenderManager mRenderManager;

    };

    extern SceneManager *gSceneManager;

} // namespace Berserk

#endif //BERSERKENGINE_SCENEMANAGER_H