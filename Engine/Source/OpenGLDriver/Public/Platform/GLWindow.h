//
// Created by Egor Orachyov on 21.02.2019.
//

#ifndef BERSERK_GLWINDOW_H
#define BERSERK_GLWINDOW_H

#include "Platform/IWindow.h"
#include "GLFW/glfw3.h"

namespace Berserk
{

    namespace Resources
    {

        /**
         * OpenGL (GLFW) platform window system implementation
         */
        class GRAPHICS_API GLWindow : public IWindow
        {
        public:

            /** Release window after usage (i.e. destroy) */
            ~GLWindow() override;

            /** @copydoc IWindow::initialize() */
            void initialize(const WindowSetup& info) override;

            /** @copydoc IWindow::setPosition() */
            void setPosition(uint32 x, uint32 y) override;

            /** @copydoc IWindow::setSize() */
            void setSize(uint32 width, uint32 height) override;

            /** @copydoc IWindow::setMaxSize() */
            void setMaxSize(uint32 width, uint32 height) override;

            /** @copydoc IWindow::setMovable() */
            void setMovable(bool flag) override;

            /** @copydoc IWindow::setResizable() */
            void setResizable(bool flag) override;

            /** @copydoc IWindow::focuse() */
            void focuse() override;

            /** @copydoc IWindow::maximize() */
            void maximize() override;

            /** @copydoc IWindow::update() */
            void update() override;

        public:

            /** @copydoc IWindow::getPosition() */
            void getPosition(uint32& x, uint32& y) override;

            /** @copydoc IWindow::getSize() */
            void getSize(uint32& width, uint32& height) override;

            /** @copydoc IWindow::getFrameBufferSize() */
            void getFrameBufferSize(uint32& width, uint32& height) override;

            /** @copydoc IWindow::shouldClose() */
            const bool shouldClose() override;

            /** @copydoc IWindow::getSizeChanged() */
            const bool getSizeChanged() override;

            /** @copydoc IWindow::getName() */
            const char* getName() override;

            /** @copydoc IWindow::getWindowInfo() */
            const WindowInfo& getWindowInfo() override;

        private:

            friend class GLRenderDriver;

            uint32 mWidth;
            uint32 mHeight;

            uint32 mMaxWidth;
            uint32 mMaxHeight;

            uint32 mOldWidth;
            uint32 mOldHeight;

            uint32 mFboWidth;
            uint32 mFboHeight;

            uint32 mPosX;
            uint32 mPosY;

            bool mIsSizeChanged : 1;
            bool mIsMovable     : 1;
            bool mIsResizable   : 1;
            bool mIsFullScreen  : 1;

            GLFWwindow* mHandler;

            CName mWindowTitle;

        };

    } // namespace Resources

} // namespace Berserk

#endif //BERSERK_GLWINDOW_H