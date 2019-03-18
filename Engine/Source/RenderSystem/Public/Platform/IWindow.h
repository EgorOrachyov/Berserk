//
// Created by Egor Orachyov on 21.02.2019.
//

#ifndef BERSERK_IWINDOW_H
#define BERSERK_IWINDOW_H

#include "Strings/StaticString.h"
#include "Misc/UsageDescriptors.h"


namespace Berserk
{

    namespace Resources
    {
        /**
         * Window provides platform independent access to native os
         * window with its handler. Allows to get common information about window's
         * properties: size, position, caption, frame buffer size ...
         */
        class GRAPHICS_API IWindow
        {
        public:

            /**
             * Specify initial values to setup and configure created
             * window. Default values could be used instead.
             */
            struct WindowSetup
            {
                WindowSetup() : caption("Default Window") {}

                uint32 width = 640;
                uint32 height = 360;

                uint32 maxWidth = 640;
                uint32 maxHeight = 360;

                uint32 posX = 100;
                uint32 posY = 100;

                bool movable = true;
                bool resizable = false;
                bool fullScreen = false;

                CName caption;
            };

            /**
             * Provides the whole information about that created
             * window. Read only access.
             */
            struct WindowInfo
            {
                uint32 width = 0;
                uint32 height = 0;

                uint32 maxWidth = 0;
                uint32 maxHeight = 0;

                uint32 fboWidth = 0;
                uint32 fboHeight = 0;

                uint32 posX = 0;
                uint32 posY = 0;

                bool isSizeChanged = false;
                bool isMovable = false;
                bool isResizable = false;
                bool iSFullScreen = false;
            };

        public:

            /** Initialize and configure window with Setup structure */
            virtual void initialize(const WindowSetup& info) = 0;

            /** Release window after usage (i.e. destroy) */
            virtual void release() = 0;

            /** Explicitly set position of the window on the screen */
            virtual void setPosition(uint32 x, uint32 y) = 0;

            /** Explicitly set size of window (it will change the size of the frame buffer) */
            virtual void setSize(uint32 width, uint32 height) = 0;

            /** Explicitly set max size of window (it will change the size of the frame buffer) */
            virtual void setMaxSize(uint32 width, uint32 height) = 0;

            /** Allow movement of the window */
            virtual void setMovable(bool flag) = 0;

            /** Allow resizing of the window */
            virtual void setResizable(bool flag) = 0;

            /** Return window in foucus */
            virtual void focuse() = 0;

            /** Maximize window on the screen */
            virtual void maximize() = 0;

            /** Should be explicitly called by render driver for each frame of the rendering cycle*/
            virtual void update() = 0;

        public:

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

            /** @return True if window should be closed */
            virtual const bool shouldClose() = 0;

            /** @return True if size was changed from the previous update */
            virtual const bool getSizeChanged() = 0;

            /** @return C string window name (name printed on the window) */
            virtual const char* getName() = 0;

            /** @return Reference to the window info */
            virtual const WindowInfo& getWindowInfo() = 0;

        };

    } // namespace Resources

} // namespace Berserk

#endif //BERSERK_IWINDOW_H