//
// Created by Egor Orachyov on 2019-08-14.
//

#ifndef BERSERK_RENDERSCENE_H
#define BERSERK_RENDERSCENE_H

#include <Rendering/IRenderScene.h>
#include <RenderSceneInfo.h>

namespace Berserk
{

    class ENGINE_API RenderScene final : public IRenderScene
    {
    public:

        RenderScene(IAllocator& allocator = Allocator::get());

        ~RenderScene() override = default;

        void addCamera(const CameraComponent &camera) override;

        void removeCamera(const CameraComponent &camera) override;

        void updateCamera(const CameraComponent &camera) override;

        void addLight(const LightComponent &light) override;

        void removeLight(const LightComponent &light) override;

        void updateLight(const LightComponent &light) override;

        void addRenderable(const RenderComponent &object) override;

        void removeRenderable(const RenderComponent &object) override;

        void updateRenderable(const RenderComponent &object) override;

    private:

        TArray<CameraSceneInfoRef> mCameras;
        TArray<DirLightSceneInfoRef> mDirLights;
        TArray<RenderableSceneInfoRef> mRenderables;

    };

} // namespace Berserk

#endif //BERSERK_RENDERSCENE_H