/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_WINDOW_H
#define BERSERK_WINDOW_H

#include <Math/Point2i.h>
#include <String/CString.h>
#include <Containers/TArray.h>

namespace Berserk {

    /** Describes window state */
    enum class EWindowState {
        Normal,
        Minimised,
        Closed
    };

    /** Main thread only window resize listener */
    class WindowResizeListener {
    public:
        /**
         * Called on main thread when the window is resized
         * @param oldSize Previous window size in pixels
         * @param newSize New window size in pixels
         */
        virtual void onResized(Size2i oldSize, Size2i newSize) = 0;
    };

    /** Main thread only window movement listener */
    class WindowPositionListener {
    public:
        /**
         * Called on main thread when the window is moved
         * @param oldPosition Previous window position in display
         * @param newPosition New position in the display pixels
         */
        virtual void onMoved(Point2i oldPosition, Point2i newPosition) = 0;
    };

    /** Main thread only window state listener */
    class WindowStateListener {
    public:
        /**
         * Called on main thread when the state of the window is changed
         * @param oldState Previous window state
         * @param newState New window state
         */
        virtual void onStateChanged(EWindowState oldState, EWindowState newState) = 0;
    };

    /**
     * @brief Platform Window
     *
     * Represents specific for OS window with default set of features and properties,
     * such as position, size, movement, events, focus and etc.
     *
     * @note Main thread only
     */
    class Window {
    public:

        virtual ~Window() = default;

        /** Makes window context current (actual for open gl devices) */
        virtual void makeRenderContextCurrent() const = 0;

        /** Set window size limits */
        virtual void setLimits(Size2i min, Size2i max) = 0;

        /** Request focus on the window */
        virtual void requestFocus() = 0;

        /** Request window close (does not actually close or dispose window) */
        virtual void requestClose() = 0;

        /** Close window, free native os resource. Window will disappear for user perspective. */
        virtual void explicitClose() = 0;

        /** @return True if close was requested on the window */
        virtual bool shouldClose() const = 0;

        /** @return True if window focused */
        virtual bool isFocused() const = 0;

        /** @return True if window closed */
        bool isClosed() const { return mState == EWindowState::Closed; }

        /** @return Unique window name for look-up in manager */
        const CString& getName() const { return mName; }

        /** @return Window caption shown to the user */
        const CString& getCaption() const { return mCaption; }

        /** @return Window frame area size in pixels */
        Size2i getSize() const  { return mSize; }

        /** @return Window min frame area size in pixels */
        Size2i getMinSize() const { return mMinSize; }

        /** @return Window max frame area size in pixels */
        Size2i getMaxSize() const  { return mMaxSize; }

        /** @return Window position on the screen in pixels */
        Point2i getPosition() const  { return mPosition; }

        /** @return Window current state */
        EWindowState getState() const { return mState; }

        /** Adds event listener to the window */
        void addResizeListener(WindowResizeListener& listener);

        /** Removes event listener from window */
        void removeResizeListener(WindowResizeListener& listener);

        /** Adds event listener to the window */
        void addPositionListener(WindowPositionListener& listener);

        /** Removes event listener from window */
        void removePositionListener(WindowPositionListener& listener);

        /** Adds event listener to the window */
        void addStateListener(WindowStateListener& listener);

        /** Removes event listener from window */
        void removeStateListener(WindowStateListener& listener);

    protected:

        Size2i mSize;
        Size2i mMinSize;
        Size2i mMaxSize;
        Point2i mPosition;
        CString mName;
        CString mCaption;
        EWindowState mState = EWindowState::Normal;
        TArray<WindowResizeListener*> mResizeListeners;
        TArray<WindowPositionListener*> mPositionListeners;
        TArray<WindowStateListener*> mStateListeners;

    };

};

#endif //BERSERK_WINDOW_H
