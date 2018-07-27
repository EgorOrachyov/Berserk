//
// Created by Egor Orachyov on 26.07.2018.
//

#include "Managers/RenderManager.h"

namespace Berserk
{

    RenderManager::RenderManager()
    {
        mCamera = NULL;
        mSpotLights.init(LightInfo::LI_MAX_SPOT_LIGHTS);
        mPointLights.init(LightInfo::LI_MAX_POINT_LIGHTS);
        mDirectionalLights.init(LightInfo::LI_MAX_DIRECTIONAL_LIGHTS);

        mSpotLights.lock();
        mPointLights.lock();
        mDirectionalLights.lock();
    }

    RenderManager::~RenderManager()
    {
        update();

        mSpotLights.empty();
        mPointLights.empty();
        mDirectionalLights.empty();
    }

    void RenderManager::update()
    {
        mCamera = NULL;
        mSpotLights.clean();
        mPointLights.clean();
        mDirectionalLights.clean();
    }

    void RenderManager::queueCamera(CameraComponent *camera)
    {
        mCamera = camera;
    }

    void RenderManager::queueLight(SpotLightComponent *light)
    {
        ASSERT(light, "Null pointer light");

        if (light == NULL)
        {
            WARNING("Null pointer light");
            return;
        }

        mSpotLights.add(light);
    }

    void RenderManager::queueLight(PointLightComponent *light)
    {
        ASSERT(light, "Null pointer light");

        if (light == NULL)
        {
            WARNING("Null pointer light");
            return;
        }

        mPointLights.add(light);
    }

    void RenderManager::queueLight(DirectionalLightComponent *light)
    {
        ASSERT(light, "Null pointer light");

        if (light == NULL)
        {
            WARNING("Null pointer light");
            return;
        }

        mDirectionalLights.add(light);
    }

    const CameraComponent* RenderManager::getCamera() const
    {
        return mCamera;
    }

    const Queue<SpotLightComponent*> &RenderManager::getSpotLights() const
    {
        return mSpotLights;
    }

    const Queue<PointLightComponent*> &RenderManager::getPointLights() const
    {
        return mPointLights;
    }

    const Queue<DirectionalLightComponent*> &RenderManager::getDirectionalLights() const
    {
        return mDirectionalLights;
    }

} // namespace Berserk