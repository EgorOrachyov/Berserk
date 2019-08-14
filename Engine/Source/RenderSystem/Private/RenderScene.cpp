//
// Created by Egor Orachyov on 2019-08-14.
//

#include "RenderScene.h"
#include <Logging/DebugLogMacros.h>

namespace Berserk
{

    RenderScene::RenderScene(Berserk::IAllocator &allocator)
        : mCameras(allocator),
          mDirLights(allocator),
          mRenderables(allocator)
    {

    }

    void RenderScene::addCamera(const CameraComponent &camera)
    {
        if (camera.attachedToRenderScene())
        {
            DEBUG_LOG_ERROR("Camera has already attached to the scene [camera: %s]",
                            camera.getObjectName().get());
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
            DEBUG_LOG_ERROR("Light has already attached to the scene [light: %s]",
                            light.getObjectName().get());
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
            DEBUG_LOG_ERROR("Render object has already attached to the scene [object: %s]",
                    object.getObjectName().get());
            return;
        }
    }

    void RenderScene::removeRenderable(const RenderComponent &object) {

    }

    void RenderScene::updateRenderable(const RenderComponent &object) {

    }

} // namespace Berserk