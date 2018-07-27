//
// Created by Egor Orachyov on 26.07.2018.
//

#include "Managers/RenderManager.h"

namespace Berserk
{

    RenderManager::RenderManager()
    {
        mSpotLights.init(LightInfo::LI_MAX_SPOT_LIGHTS);
        mPointLights.init(LightInfo::LI_MAX_POINT_LIGHTS);
        mDirectionalLights.init(LightInfo::LI_MAX_DIRECTIONAL_LIGHTS);
    }

    RenderManager::~RenderManager()
    {

    }

    void RenderManager::queueLight(SpotLight *light)
    {

    }

    void RenderManager::queueLight(PointLight *light)
    {

    }

    void RenderManager::queueLight(DirectionalLight *light)
    {

    }

} // namespace Berserk