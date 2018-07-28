//
// Created by Egor Orachyov on 26.07.2018.
//

#include "Managers/RenderManager.h"

namespace Berserk
{
    BaseRenderMaterialComponent::BaseRenderMaterialComponent(RenderComponent *renderComponent,
                                                             BaseMaterialComponent *materialComponent)
    {
        mRenderComponent = renderComponent;
        mMaterialComponent = materialComponent;
    }

    RenderManager::RenderManager()
    {
        mCamera = NULL;
        mSpotLights.init(LightInfo::LI_MAX_SPOT_LIGHTS);
        mPointLights.init(LightInfo::LI_MAX_POINT_LIGHTS);
        mDirectionalLights.init(LightInfo::LI_MAX_DIRECTIONAL_LIGHTS);

        mSpotLights.lock();
        mPointLights.lock();
        mDirectionalLights.lock();

        mBaseRenderMaterials.init();
    }

    RenderManager::~RenderManager()
    {
        update();

        mSpotLights.empty();
        mPointLights.empty();
        mDirectionalLights.empty();

        mBaseRenderMaterials.empty();
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
        ASSERT(light, "NULL pointer light");

        if (light == NULL)
        {
            WARNING("NULL pointer light");
            return;
        }

        mSpotLights.add(light);
    }

    void RenderManager::queueLight(PointLightComponent *light)
    {
        ASSERT(light, "NULL pointer light");

        if (light == NULL)
        {
            WARNING("NULL pointer light");
            return;
        }

        mPointLights.add(light);
    }

    void RenderManager::queueLight(DirectionalLightComponent *light)
    {
        ASSERT(light, "NULL pointer light");

        if (light == NULL)
        {
            WARNING("NULL pointer light");
            return;
        }

        mDirectionalLights.add(light);
    }

    void RenderManager::queueRenderComponent(RenderComponent *renderComponent)
    {
        ASSERT(renderComponent, "NULL render component");

        if (renderComponent == NULL)
        {
            WARNING("NULL render component");
            return;
        }

        mTmpRenderComponent = renderComponent;
    }

    void RenderManager::queueMaterial(BaseMaterialComponent *materialComponent)
    {
        ASSERT(materialComponent, "NULL material component");

        if (materialComponent == NULL)
        {
            WARNING("NULL material component");
            return;
        }

        mBaseRenderMaterials.add(BaseRenderMaterialComponent(mTmpRenderComponent, materialComponent));
    }

    const CameraComponent* RenderManager::getCamera() const
    {
        return mCamera;
    }

    const List<SpotLightComponent*> &RenderManager::getSpotLights() const
    {
        return mSpotLights;
    }

    const List<PointLightComponent*> &RenderManager::getPointLights() const
    {
        return mPointLights;
    }

    const List<DirectionalLightComponent*> &RenderManager::getDirectionalLights() const
    {
        return mDirectionalLights;
    }

    const List<BaseRenderMaterialComponent>& RenderManager::getBaseRenderMaterials() const
    {
        return mBaseRenderMaterials;
    }

} // namespace Berserk