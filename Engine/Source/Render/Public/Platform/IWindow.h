//
// Created by Egor Orachyov on 21.02.2019.
//

#ifndef BERSERK_IWINDOW_H
#define BERSERK_IWINDOW_H

namespace Berserk
{

    /**
     * Window provides platform independent access to native os
     * window with its handler. Allows to get common information about window's
     * properties: size, position, caption, frame buffer size ...
     */
    class IWindow
    {
    public:

        /**
         * Specify initial values to setup and configure created
         * window. Default values could be used instead.
         */
        struct WindowSetup
        {
            uint32 width = 640;
            uint32 height = 480;
            uint32 posX = 100;
            uint32 posY = 100;

            bool movable = false;
            bool resizable = false;
            bool fullScreen = false;

            CName caption = "Default Window";
        };

        /**
         * Provides the whole information about that created
         * window. Read only access.
         */
        struct WindowInfo
        {
            uint32 width;
            uint32 height;

            uint32 fboWidth;
            uint32 fboHeight;

            uint32 posX;
            uint32 posY;

            bool isSizeChanged;
            bool isMovable;
            bool isResizable;
            bool iSFullScreen;
        };

    public:

        /** Initialize and configure window with Setup structure */
        virtual void initialize(const WindowSetup& info) = 0;

        /** Explicitly set position of the window on the screen */
        virtual void setPosition(uint32 x, uint32 y) = 0;

        /** Explicitly set size of window (it will change the size of the frame buffer) */
        virtual void setSize(uint32 width, uint32 height) = 0;

        /** Allow movement of the window */
        virtual void setMovable(bool flag) = 0;

        /** Allow resizing of the window */
        virtual void setResizable(bool flag) = 0;

        /** Should be explicitly called by render driver for each frame of the rendering cycle*/
        virtual void update() = 0;

        /**
         * Get window position on the os desktop screen
         * @param[out] x
         * @param[out] y
         */
        virtual void getPosition(uint32& x, uint32& y) = 0;

        /**
         * Size of the window in the `special` units (platform specific units)
         * @param[out] width
         * @param[out] height
         */
        virtual void getSize(uint32& width, uint32& height) = 0;

        /**
         * Size of the window frame buffer in the pixels
         * @param[out] width
         * @param[out] height
         */
        virtual void getFrameBufferSize(uint32& width, uint32& height) = 0;

        /** @return True if size was changed from the previous update */
        virtual const bool getSizeChanged() = 0;

        /** @return C string window name (name printed on the window) */
        virtual const char* getName();

        /** @return Reference to the window info */
        virtual const WindowInfo& getWindowInfo() = 0;

    };

} // namespace Berserk

#endif //BERSERK_IWINDOW_H