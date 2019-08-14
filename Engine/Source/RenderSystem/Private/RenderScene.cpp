//
// Created by Egor Orachyov on 2019-08-14.
//

#include "RenderScene.h"
#include <Logging/DebugLogMacros.h>

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
            DEBUG_LOG_ERROR("Camera has already attached to the scene [scene: %s] [camera: %s]",
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
            DEBUG_LOG_ERROR("Light has already attached to the scene [scene: %s] [light: %s]",
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
            DEBUG_LOG_ERROR("Render object has already attached to the scene [scene: %s] [object: %s]",
                            mSceneName.get(), object.getObjectName().get());
            return;
        }

        auto data = mAllocator.engine_new_no_args<RenderableSceneInfo>();

        data->localToWorld = Transform::convertToMat4x4f(object.getWorldRotation(), object.getWorldPosition());
        data->renderable = object.getRenderable();
        data->castShadows = object.castShadows();
        data->applyCulling = object.applyCulling();
        data->drawWireframe = object.drawWireframe();
        data->drawWireframeOnly = object.drawWireframeOnly();
        data->drawBoundingVolume = object.drawBoundingVolume();
        data->isActive = object.isActive();

        const TArray<MeshNode> &nodes = data->renderable->getMesh()->getMeshNodes();
        const TArray<MaterialHandle> &materials = data->renderable->getMaterials();
        const TArray<Mat4x4f> &transforms = data->renderable->getTransformations();

        RHIGeometryBufferRef geometryBuffer;

        for (auto node = nodes.begin(); node != nullptr; node = nodes.next())
        {
            MaterialHandle material = mDefaultMaterial;
            Mat4x4f transform;

            if (node->isMaterialPresent())
                material = materials.get(node->getMaterialIndex());

            if (node->isTransformPresent())
                transform = transforms.get(node->getTransformIndex());

            data->renderElements.emplace(
                    data->renderable->getMesh(),
                    *node,
                    material,
                    transform,
                    geometryBuffer);
        }

        mRenderables.emplace(data, &mAllocator);
    }

    void RenderScene::removeRenderable(const RenderComponent &object) {

    }

    void RenderScene::updateRenderable(const RenderComponent &object) {

    }

} // namespace Berserk