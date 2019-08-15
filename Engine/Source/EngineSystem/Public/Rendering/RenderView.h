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
     *
     * @note Because of the deferred nature of the rendering, render system could
     *       use tmp buffer to create a result image. Therefore size of the
     *       framebuffer and size of the view region on the screen could differ
     *
     * @note Use framebuffer size to specify size of the internal buffer in pixels
     *
     * @note Use viewport size to specify view region on the window
     */
    class ENGINE_API RenderView : public Allocatable
    {
    public:

        void setViewport(const Rect& viewPort) { mViewport = viewPort; }

        void setFramebuffer(const Rect& frameBuffer) { mFramebuffer = frameBuffer; }

        void setWindow(const WindowRef& window) { mWindow = window; }

        Rect getViewport() const { return mViewport; }

        Rect getFramebuffer() const { return mFramebuffer; }

        WindowRef getWindow() const { return mWindow; }

    protected:

        /** Size of the framebuffer */
        Rect mFramebuffer;

        /** Viewport to sent to RHI */
        Rect mViewport;

        /** Used window for rendering */
        WindowRef mWindow;

    };

} // namespace Berserk

#endif //BERSERK_RENDERVIEW_H