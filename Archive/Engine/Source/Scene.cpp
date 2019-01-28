//
// Created by Egor Orachyov on 28.07.2018.
//

#include "Objects/Scene/Scene.h"
#include "Math/UtilityMatrices.h"
#include "Misc/Delete.h"

namespace Berserk
{

    Scene::Scene(const CStaticString &name) : Object(name)
    {
        CHAR buffer[Buffers::SIZE_16] = {'\0'};
        CStaticString rootId;

        for(UINT32 i = 0; i < SceneInfo::SI_MAX_NUM_OF_LAYOUTS; i++)
        {
            sprintf(buffer, "Root%u", i);
            rootId.copy(buffer);

            mRoots[i] = new Actor(name + buffer);
        }
    }

    Scene::~Scene()
    {
        for(UINT32 i = 0; i < SceneInfo::SI_MAX_NUM_OF_LAYOUTS; i++)
        { SAFE_DELETE(mRoots[i]); }
    }

    void Scene::onInit()
    {

    }

    void Scene::onUpdate(FLOAT64 delta)
    {

    }

    void Scene::onReset()
    {

    }

    void Scene::onDestroy()
    {

    }

    Actor &Scene::getRoot(UINT32 layout)
    {
        ASSERT(layout < SceneInfo::SI_MAX_NUM_OF_LAYOUTS, "Layout id out of range");

        return *mRoots[layout];
    }

    void Scene::init()
    {
        for (UINT32 i = 0; i < SceneInfo::SI_MAX_NUM_OF_LAYOUTS; i++)
        { mRoots[i]->init(); }
    }

    void Scene::process(UINT32 layout, FLOAT64 delta)
    {
        ASSERT(layout < SceneInfo::SI_MAX_NUM_OF_LAYOUTS, "Layout id out of range");
        mRoots[layout]->process(delta, newMatrix(1));
    }

    void Scene::reset()
    {
        for (UINT32 i = 0; i < SceneInfo::SI_MAX_NUM_OF_LAYOUTS; i++)
        { mRoots[i]->init(); }
    }

    void Scene::destroy()
    {
        for (UINT32 i = 0; i < SceneInfo::SI_MAX_NUM_OF_LAYOUTS; i++)
        { mRoots[i]->init(); }
    }
    
} // namespace Berserk