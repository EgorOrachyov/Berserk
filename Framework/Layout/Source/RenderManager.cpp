//
// Created by Egor Orachyov on 26.07.2018.
//

#include "Managers/RenderManager.h"

namespace Berserk
{
    BaseRenderMaterialComponent::BaseRenderMaterialComponent(Matrix4x4f *matrix4x4f, GPUBuffer *buffer,
                                                             BaseMaterialComponent *materialComponent)
    {
        mTransformationComponent = matrix4x4f;
        mRenderBufferComponent = buffer;
        mMaterialComponent = materialComponent;
    }

    RenderManager::RenderManager()
    {
        mTmpBuffer = NULL;
        mTmpMatrix4x4f = NULL;

        mCamera = NULL;
        mAmbientLight = NULL;
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
        mTmpBuffer = NULL;
        mTmpMatrix4x4f = NULL;
        mCamera = NULL;
        mAmbientLight = NULL;
        mSpotLights.clean();
        mPointLights.clean();
        mDirectionalLights.clean();
        mBaseRenderMaterials.clean();
    }

    void RenderManager::queueCamera(CameraComponent *camera)
    {
        ASSERT(camera, "NULL camera pass for render manager");

        if (camera == NULL)
        {
            WARNING("NULL camera pass for render manager");
            return;
        }

        mCamera = camera;
    }

    void RenderManager::queueLight(AmbientLightComponent *light)
    {
        ASSERT(light, "NULL pointer light");

        if (light == NULL)
        {
            WARNING("NULL pointer light");
            return;
        }

        mAmbientLight = light;
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

    void RenderManager::queueBuffer(GPUBuffer *buffer)
    {
        ASSERT(buffer, "NULL pointer GPU buffer");

        if (buffer == NULL)
        {
            WARNING("NULL pointer GPU buffer");
            return;
        }

        mTmpBuffer = buffer;
    }

    void RenderManager::queueTransformation(Matrix4x4f *matrix4x4f)
    {
        ASSERT(matrix4x4f, "NULL pointer transformation matrix 4x4f");

        if (matrix4x4f == NULL)
        {
            WARNING("NULL pointer transformation matrix 4x4f");
            return;
        }

        mTmpMatrix4x4f = matrix4x4f;
    }

    void RenderManager::queueMaterial(BaseMaterialComponent *materialComponent)
    {
        ASSERT(materialComponent, "NULL material component");

        if (materialComponent == NULL)
        {
            WARNING("NULL material component");
            return;
        }

        mBaseRenderMaterials.add(BaseRenderMaterialComponent(mTmpMatrix4x4f, mTmpBuffer, materialComponent));
    }

    const CameraComponent* RenderManager::getCamera() const
    {
        return mCamera;
    }

    const AmbientLightComponent* RenderManager::getAmbientLight() const
    {
        return mAmbientLight;
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