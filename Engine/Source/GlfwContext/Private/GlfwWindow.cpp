//
// Created by Egor Orachyov on 2019-07-17.
//

#include "GlfwWindow.h"

namespace Berserk
{

    GlfwWindow::GlfwWindow(Berserk::uint32 width, Berserk::uint32 height, const Berserk::String &name)
        : mName(name), mWidth(width), mHeight(height)
    {
        mWindowHandler = glfwCreateWindow(width, height, name.get(), nullptr, nullptr);

        update();

        if (!mIsResizable)
        {
            glfwSetWindowSizeLimits(mWindowHandler, width, height, width, height);
        }
    }

    GlfwWindow::~GlfwWindow()
    {
        if (mWindowHandler)
        {
            glfwDestroyWindow(mWindowHandler);
            mWindowHandler = nullptr;
        }
    }

    void GlfwWindow::setPosition(uint32 x, uint32 y)
    {
        CriticalSection section(mMutex);

        if (mIsMovable)
        {
            mPosX = x;
            mPosY = y;

            glfwSetWindowPos(mWindowHandler, x, y);

            mIsPosChanged = true;
        }
    }

    void GlfwWindow::setSize(uint32 width, uint32 height)
    {
        CriticalSection section(mMutex);

        if (mIsResizable)
        {
            mWidth = width;
            mHeight = height;

            glfwSetWindowSize(mWindowHandler, width, height);
            glfwGetFramebufferSize(mWindowHandler, (int32*)&mWidth_Framebuffer, (int32*)&mHeight_Framebuffer);

            mIsSizeChanged = true;
        }
    }

    void GlfwWindow::maximize()
    {
        CriticalSection section(mMutex);
        glfwMaximizeWindow(mWindowHandler);
    }

    void GlfwWindow::minimize()
    {
        CriticalSection section(mMutex);
        glfwHideWindow(mWindowHandler);
    }

    void GlfwWindow::focus()
    {
        CriticalSection section(mMutex);
        glfwFocusWindow(mWindowHandler);
    }

    void Berserk::GlfwWindow::close()
    {
        CriticalSection section(mMutex);
        mShouldClose = true;
    }

    void GlfwWindow::makeActiveRenderingTarget()
    {
        CriticalSection section(mMutex);
        glfwMakeContextCurrent(mWindowHandler);
    }

    uint32 GlfwWindow::getWidth() const
    {
        return mWidth;
    }

    uint32 GlfwWindow::getHeight() const
    {
        return mHeight;
    }

    uint32 GlfwWindow::getWidth_FrameBuffer() const
    {
        return mWidth_Framebuffer;
    }

    uint32 GlfwWindow::getHeight_FrameBuffer() const
    {
        return mHeight_Framebuffer;
    }

    uint32 GlfwWindow::getPosX() const
    {
        return mPosX;
    }

    uint32 GlfwWindow::getPosY() const
    {
        return mPosY;
    }

    bool GlfwWindow::isMovable() const
    {
        return mIsMovable;
    }

    bool GlfwWindow::isResizable() const
    {
        return mIsResizable;
    }

    bool GlfwWindow::isSizeChanged() const
    {
        return mIsSizeChanged;
    }

    bool GlfwWindow::isPositionChanged() const
    {
        return mIsPosChanged;
    }

    bool Berserk::GlfwWindow::shouldClose() const
    {
        return mShouldClose;
    }

    const String &GlfwWindow::getName() const
    {
        return mName;
    }

    void GlfwWindow::update()
    {
        CriticalSection section(mMutex);

        auto newPosX = mPosX;
        auto newPosY = mPosY;

        auto mOldWidth = mWidth;
        auto mOldHeight = mHeight;

        mIsSizeChanged = false;
        mIsPosChanged = false;

        glfwGetWindowPos(mWindowHandler, (int32*)&newPosX, (int32*)&newPosY);
        glfwGetWindowSize(mWindowHandler, (int32*)&mWidth, (int32*)&mHeight);
        glfwGetFramebufferSize(mWindowHandler, (int32*)&mWidth_Framebuffer, (int32*)&mHeight_Framebuffer);

        mIsSizeChanged = mOldWidth != mWidth || mOldHeight != mHeight;
        mIsPosChanged = newPosX != mPosX || newPosY != mPosY;

        if (!mIsMovable && mIsPosChanged)
        {
            glfwSetWindowPos(mWindowHandler, mPosX, mPosY);
        }
        else
        {
            mPosX = newPosX;
            mPosY = newPosY;
        }

        mShouldClose = glfwWindowShouldClose(mWindowHandler) || mShouldClose;
    }

} // namespace Berserk