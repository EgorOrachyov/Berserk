//
// Created by Egor Orachyov on 21.02.2019.
//

#ifndef BERSERK_GLWINDOW_H
#define BERSERK_GLWINDOW_H

#include "Platform/IWindow.h"
#include "GLFW/glfw3.h"

namespace Berserk
{

    /**
     * OpenGL (GLFW) platform window system implementation
     */
    class GRAPHICS_API GLWindow : public IWindow
    {
    public:

        /** Initialize and configure window with Setup structure */
        void initialize(const WindowSetup& info) override;

        /** Release window after usage (i.e. destroy) */
        void release() override;

        /** Explicitly set position of the window on the screen */
        void setPosition(uint32 x, uint32 y) override;

        /** Explicitly set size of window (it will change the size of the frame buffer) */
        void setSize(uint32 width, uint32 height) override;

        void setMaxSize(uint32 width, uint32 height) override;

        /** Allow movement of the window */
        void setMovable(bool flag) override;

        /** Allow resizing of the window */
        void setResizable(bool flag) override;

        /** Return window in foucus */
        void focuse() override;

        /** Maximize window on the screen */
        void maximize() override;

        /** Should be explicitly called by render driver for each frame of the rendering cycle*/
        void update() override;

    public:

        /**
         * Get window position on the os desktop screen
         * @param[out] x
         * @param[out] y
         */
        void getPosition(uint32& x, uint32& y) override;

        /**
         * Size of the window in the `special` units (platform specific units)
         * @param[out] width
         * @param[out] height
         */
        void getSize(uint32& width, uint32& height) override;

        /**
         * Size of the window frame buffer in the pixels
         * @param[out] width
         * @param[out] height
         */
        void getFrameBufferSize(uint32& width, uint32& height) override;

        /** @return True if window should be closed */
        const bool shouldClose() override;

        /** @return True if size was changed from the previous update */
        const bool getSizeChanged() override;

        /** @return C string window name (name printed on the window) */
        const char* getName() override;

        /** @return Reference to the window info */
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

} // namespace Berserk

#endif //BERSERK_GLWINDOW_H