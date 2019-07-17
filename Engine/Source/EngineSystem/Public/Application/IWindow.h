//
// Created by Egor Orachyov on 2019-07-17.
//

#ifndef BERSERK_IWINDOW_H
#define BERSERK_IWINDOW_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Resource/TSharedPtr.h>
#include <Strings/String.h>

namespace Berserk
{

    /** Primary engine window interface to get access to view windows */
    class ENGINE_API IWindow
    {
    public:

        virtual ~IWindow() = default;

        /** Set window position on the screen */
        virtual void setPosition(uint32 x, uint32 y) = 0;

        /** Set window size (if it is resizeable) */
        virtual void setSize(uint32 width, uint32 height) = 0;

        /** Maximize window size on the scree */
        virtual void maximize() = 0;

        /** Minimize window size (hide in the bar) */
        virtual void minimize() = 0;

        /** Return window in the focus */
        virtual void focus() = 0;

        /** Request close for this window */
        virtual void close() = 0;

        /** Makes this window as active to render into that */
        virtual void makeActiveRenderingTarget() = 0;

        /** @return Window width */
        virtual uint32 getWidth() const = 0;

        /** @return Window height */
        virtual uint32 getHeight() const = 0;

        /** @return Window framebuffer width (for rendering purposes) */
        virtual uint32 getWidth_FrameBuffer() const = 0;

        /** @return Window framebuffer height (for rendering purposes) */
        virtual uint32 getHeight_FrameBuffer() const = 0;

        /** @return Window position x on the screen */
        virtual uint32 getPosX() const = 0;

        /** @return Window position y on the screen */
        virtual uint32 getPosY() const = 0;

        /** @return True, whether window is movable on screen */
        virtual bool isMovable() const = 0;

        /** @return True, whether can change window size */
        virtual bool isResizable() const = 0;

        /** @return True, whether window size was changed since last update [loop] */
        virtual bool isSizeChanged() const = 0;

        /** @return True, whether window position was changed since last update [loop] */
        virtual bool isPositionChanged() const = 0;

        /** @return True, if window were requested to be closed */
        virtual bool shouldClose() const = 0;

        /** @return Window caption/name */
        virtual const String& getName() const = 0;

    };

    typedef TSharedPtr<IWindow> IWindowRef;

} // namespace Berserk

#endif //BERSERK_IWINDOW_H