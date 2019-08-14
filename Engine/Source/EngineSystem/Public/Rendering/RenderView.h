//
// Created by Egor Orachyov on 2019-08-06.
//

#ifndef BERSERK_RENDERVIEW_H
#define BERSERK_RENDERVIEW_H

#include <Engine/EngineObjects.h>
#include <Application/IWindow.h>
#include <Rendering/RenderingCommon.h>
#include <RenderSceneInfo.h>

namespace Berserk
{


    /**
     * Defines view rendering properties of the
     * rendering the scene to single camera.
     */
    class ENGINE_API RenderView final : public Allocatable
    {
    public:

        void setViewport(const Viewport& viewPort) { mViewport = viewPort; }

        void setCameraInfo(const CameraSceneInfoRef& camera) { mCamera = camera; }

        void setWindow(const WindowRef& window) { mWindow = window; }

        Viewport getViewport() const { return mViewport; }

        CameraSceneInfoRef getCameraInfo() const { return mCamera; }

        WindowRef getWindow() const { return mWindow; }

    protected:

        /** Viewport to sent to RHI */
        Viewport mViewport;

        /** Camera for rendering */
        CameraSceneInfoRef mCamera;

        /** Used window for rendering */
        WindowRef mWindow;

    };

} // namespace Berserk

#endif //BERSERK_RENDERVIEW_H