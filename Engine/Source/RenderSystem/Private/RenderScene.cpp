//
// Created by Egor Orachyov on 2019-08-14.
//

#include "RenderScene.h"
#include <Logging/DebugLogMacros.h>
#include <RenderUtils.h>

namespace Berserk
{

    RenderScene::RenderScene(Berserk::IAllocator &allocator)
        : mAllocator(allocator),
          mCameras(allocator),
          mDirLights(allocator),
          mRenderables(allocator)
    {

    }

    void RenderScene::addCamera(const CameraComponent &camera)
    {
        if (camera.attachedToRenderScene())
        {
            DEBUG_LOG_ERROR("RenderScene: Camera already attached to the scene [scene: %s] [camera: %s]",
                            mSceneName.get(), camera.getObjectName().get());
            return;
        }
    }

    void RenderScene::removeCamera(const CameraComponent &camera) {

    }

    void RenderScene::updateCamera(const CameraComponent &camera) {

    }

    void RenderScene::addLight(const LightComponent &light)
    {
        if (light.attachedToRenderScene())
        {
            DEBUG_LOG_ERROR("RenderScene: Light already attached to the scene [scene: %s] [light: %s]",
                            mSceneName.get(), light.getObjectName().get());
            return;
        }

        switch (light.getLightSourceType())
        {
            case ELightSourceType::LST_DirectionalLight:
                addLight_internal((DirectionalLightComponent &) light);
                break;
            default:
                DEBUG_LOG_ERROR("RenderScene: Unknown light source type [scene: %s] [light: %s]",
                                mSceneName.get(), light.getObjectName().get());
                return;
        }
    }

    void RenderScene::removeLight(const LightComponent &light) {

    }

    void RenderScene::updateLight(const LightComponent &light) {

    }

    void RenderScene::addRenderable(const RenderComponent &object)
    {
        if (object.attachedToRenderScene())
        {
            DEBUG_LOG_ERROR("RenderScene: Render object already attached to the scene [scene: %s] [object: %s]",
                            mSceneName.get(), object.getObjectName().get());
            return;
        }

        auto data = mAllocator.mem_new<RenderableSceneInfo>();

        data->localToWorld = Transform::convertToMat4x4f(object.getWorldRotation(), object.getWorldPosition());
        data->renderable = object.getRenderable();
        data->castShadows = object.castShadows();
        data->applyCulling = object.applyCulling();
        data->drawWireframe = object.drawWireframe();
        data->drawWireframeOnly = object.drawWireframeOnly();
        data->drawBoundingVolume = object.drawBoundingVolume();
        data->isActive = object.isActive();

        RenderableSceneInfoRef& ptr = mRenderables.emplace(data, &mAllocator);

        const HMesh& meshHandle = data->renderable->getMesh();
        const TArray<MeshNode> &nodes = data->renderable->getMesh()->getMeshNodes();
        const TArray<HMaterial> &materials = data->renderable->getMaterials();
        const TArray<Mat4x4f> &transforms = data->renderable->getTransformations();

        RenderUtils::createGeometryBuffer(mDriver, ptr);

        for (auto node = nodes.begin(); node != nullptr; node = nodes.next())
        {
            HMaterial material = mDefaultMaterial;
            Mat4x4f transform;

            if (node->isMaterialPresent())
                material = materials.get(node->getMaterialIndex());

            if (node->isTransformPresent())
                transform = transforms.get(node->getTransformIndex());

            data->renderElements.emplace(
                    meshHandle,
                    *node,
                    material,
                    transform,
                    data->geometry);
        }

        SceneInfo::output(*data, OutputDevice::get());
    }

    void RenderScene::removeRenderable(const RenderComponent &object)
    {

    }

    void RenderScene::updateRenderable(const RenderComponent &object)
    {

    }

    const void RenderScene::addLight_internal(const DirectionalLightComponent &light)
    {
        auto data = mAllocator.mem_new<DirLightSceneInfo>();

        data->localToWorld = Transform::convertToMat4x4f(light.getWorldRotation(), light.getWorldPosition());
        data->lightColor = light.getLightColor();
        data->castShadows = light.castShadows();
        data->isActive = light.isActive();
        data->distanceOfAction = light.getMaxLightDistance();
        data->distanceOfActionSq = data->distanceOfAction * data->distanceOfAction;
        data->worldDirection = light.getWorldDirection();

        mDirLights.emplace(data, &mAllocator);

        SceneInfo::output(*data, OutputDevice::get());
    }

} // namespace Berserk