//
// Created by Egor Orachyov on 2019-08-14.
//

#ifndef BERSERK_RENDERSCENE_H
#define BERSERK_RENDERSCENE_H

#include <Rendering/IRenderScene.h>
#include <RenderSceneInfo.h>
#include <RHI/RHIDriver.h>

namespace Berserk
{

    class RENDER_API RenderScene final : public IRenderScene
    {
    public:

        explicit RenderScene(IAllocator& allocator = Allocator::get());

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

        void setSceneName(const String& name) { mSceneName = name; }

        void setDefaultWindow(const WindowRef& window) { mDefaultWindow = window; }

        void setDefaultMaterial(const MaterialHandle& material) { mDefaultMaterial = material; }

        void setDriver(const RHIDriverRef& driver) { mDriver = driver; }

        const String& getSceneName() const { return mSceneName; }

        const WindowRef& getDefaultWindow() const { return mDefaultWindow; }

        const MaterialHandle& getDefaultMaterial() const { return mDefaultMaterial; }

        const RHIDriverRef& getDriver() const { return mDriver; }

        const TArray<DirLightSceneInfoRef>& getDirLights() const { return mDirLights; }

        const TArray<RenderableSceneInfoRef>& getRenderables() const { return mRenderables; }

    private:

        const void addLight_internal(const DirectionalLightComponent &light);

    private:

        IAllocator &mAllocator;
        String mSceneName;
        WindowRef mDefaultWindow;
        MaterialHandle mDefaultMaterial;
        RHIDriverRef mDriver;

        // todo: add pool for each kind of resources

        TArray<CameraSceneInfoRef> mCameras;
        TArray<DirLightSceneInfoRef> mDirLights;
        TArray<RenderableSceneInfoRef> mRenderables;

    };

} // namespace Berserk

#endif //BERSERK_RENDERSCENE_H