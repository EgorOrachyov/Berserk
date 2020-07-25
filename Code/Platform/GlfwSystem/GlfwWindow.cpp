/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/System.h>
#include <GlfwSystem/GlfwWindow.h>
#include <GlfwSystem/GlfwWindowManager.h>

namespace Berserk {

    GlfwWindow::GlfwWindow(const CString &name, const CString &caption, Size2i size, EWindowActionOnClose actionOnClose) {
        mMonitor = glfwGetPrimaryMonitor();
        glfwGetMonitorContentScale(mMonitor, &mScaleX, &mScaleY);
        Size2i glfwSize = Size2i((int32)((float)size[0] / mScaleX), (int32)((float)size[1] / mScaleX));

        mWindowHandle = glfwCreateWindow(glfwSize[0], glfwSize[1], caption.data(), nullptr, nullptr);
        mName = name;
        mCaption = caption;
        mState = EWindowState::Normal;
        mAction = actionOnClose;

        glfwGetWindowPos(mWindowHandle, &mPosition[0], &mPosition[1]);
        mPosition[0] = (int32)(mScaleX * mPosition[0]);
        mPosition[1] = (int32)(mScaleY * mPosition[1]);

        glfwGetFramebufferSize(mWindowHandle, &mSize[0], &mSize[1]);
        mMinSize = Size2i{0,0};
        mMaxSize = Size2i{0xfffffff,0xfffffff};
    }

    GlfwWindow::~GlfwWindow() {
        close();
        removeMyselfFromWindowManager();
    }

    void GlfwWindow::makeRenderContextCurrent() const {
        if (!isClosed()) {
            glfwMakeContextCurrent(mWindowHandle);
        }
    }

    void GlfwWindow::setLimits(Size2i min, Size2i max) {
        if (isClosed())
            return;

        max = Math::max(max, {0,0});
        min = Math::max(min, {0,0});

        max = Math::max(min,max);
        min = Math::min(min,max);

        mMaxSize = Size2i((int32)((float)max[0] / mScaleX), (int32)((float)max[1] / mScaleY));
        mMinSize = Size2i((int32)((float)min[0] / mScaleX), (int32)((float)min[1] / mScaleY));

        glfwSetWindowSizeLimits(mWindowHandle, mMinSize[0], mMinSize[1], mMaxSize[0], mMaxSize[1]);
    }

    void GlfwWindow::requestFocus() {
        if (isClosed())
            return;

        glfwFocusWindow(mWindowHandle);
    }

    void GlfwWindow::requestClose() {
        mShouldClose = true;
    }

    void GlfwWindow::explicitClose() {
        mShouldClose = mExplicitClose = true;
    }

    bool GlfwWindow::shouldClose() const {
        return mShouldClose;
    }

    bool GlfwWindow::isFocused() const {
        return mIsFocused;
    }

    void GlfwWindow::processResize(Size2i newSize) {
        if (isClosed())
            return;

        auto oldSize = mSize;
        mSize = newSize;

        for (auto listener: mResizeListeners) {
            listener->onResized(oldSize, newSize);
        }
    }

    void GlfwWindow::processMovement(Point2i newPos) {
        if (isClosed())
            return;

        auto oldPos = mPosition;
        mPosition = newPos;

        GLFWmonitor* monitor = glfwGetWindowMonitor(mWindowHandle);

        // If monitor changed, scale and pixel ration changes too
        if (monitor != mMonitor && monitor != nullptr) {
            mMonitor = monitor;
            glfwGetMonitorContentScale(mMonitor, &mScaleX, &mScaleY);
        }

        mPosition[0] = (int32)(mScaleX * mPosition[0]);
        mPosition[1] = (int32)(mScaleY * mPosition[1]);

        for (auto listener: mPositionListeners) {
            listener->onMoved(oldPos, newPos);
        }
    }

    void GlfwWindow::processIconification(EWindowState newState) {
        if (isClosed())
            return;

        auto oldState = mState;
        mState = newState;

        for (auto listener: mStateListeners) {
            listener->onStateChanged(oldState, newState);
        }
    }

    void GlfwWindow::processFocus(bool focus) {
        mIsFocused = focus;
    }

    void GlfwWindow::update() {
        if (isClosed())
            return;

        mShouldClose = glfwWindowShouldClose(mWindowHandle);
        glfwSwapBuffers(mWindowHandle);

        if (mShouldClose && mAction == EWindowActionOnClose::RequestSystemClose) {
            System::getSingleton().requestClose();
        }

        if (mExplicitClose)
            close();
    }

    void GlfwWindow::close() {
        if (!isClosed()) {
            auto oldState = mState;
            mState = EWindowState::Closed;

            for (auto listener: mStateListeners) {
                listener->onStateChanged(oldState, mState);
            }

            glfwDestroyWindow(mWindowHandle);
            mWindowHandle = nullptr;
            mMonitor = nullptr;
        }
    }

    void GlfwWindow::removeMyselfFromWindowManager() {
        if (!mInManager)
            return;

        auto& manager = GlfwWindowManager::getGlfwManager();
        auto& windows = manager.getWindows();

        windows.removeUnorderByPredicate([this](const TPtrShared<GlfwWindow> &w) -> bool {
            return w->getWindowHandleGLFW() == this->mWindowHandle;
        });

        mInManager = false;
    }

}