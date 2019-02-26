//
// Created by Egor Orachyov on 21.02.2019.
//

#include "Platform/GLWindow.h"
#include "Logging/LogMacros.h"

namespace Berserk
{

    void GLWindow::initialize(const WindowSetup &info)
    {
        mWidth = info.width;
        mHeight = info.height;

        mPosX = info.posX;
        mPosY = info.posY;

        mMaxWidth = info.maxWidth;
        mMaxHeight = info.maxHeight;

        mWindowTitle = info.caption;
        PUSH("GLWindow: initialize window [name: %s]", mWindowTitle.get());

        mIsMovable = info.movable;
        mIsResizable = info.resizable;
        mIsFullScreen = info.fullScreen;

        update();
    }

    void GLWindow::release()
    {
        glfwDestroyWindow(mHandler);
        PUSH("GLWindow: destroy window [name: %s]", mWindowTitle.get());
    }

    void GLWindow::setPosition(uint32 x, uint32 y)
    {
        if (mIsMovable)
        {
            mPosX = x;
            mPosY = y;
            glfwSetWindowPos(mHandler, x, y);
        }
    }

    void GLWindow::setSize(uint32 width, uint32 height)
    {
        if (mIsResizable)
        {
            mWidth = width;
            mHeight = height;
            glfwSetWindowSize(mHandler, width, height);
        }
    }

    void GLWindow::setMaxSize(uint32 width, uint32 height)
    {
        mMaxWidth = width;
        mMaxHeight = height;

        if (mIsResizable)
        {
            glfwSetWindowSizeLimits(mHandler, mWidth, mHeight, mMaxWidth, mMaxHeight);
        }
    }

    void GLWindow::setMovable(bool flag)
    {
        mIsMovable = flag;
    }

    void GLWindow::setResizable(bool flag)
    {
        mIsResizable = flag;
        if (mIsResizable) glfwSetWindowSizeLimits(mHandler, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE);
        else glfwSetWindowSizeLimits(mHandler, mWidth, mHeight, mWidth, mHeight);
    }

    void GLWindow::focuse()
    {
        glfwShowWindow(mHandler);
    }

    void GLWindow::maximize()
    {
        glfwMaximizeWindow(mHandler);
    }

    void GLWindow::update()
    {
        auto newPosX = mPosX;
        auto newPosY = mPosY;

        mOldWidth = mWidth;
        mOldHeight = mHeight;

        mIsSizeChanged = false;

        glfwGetWindowPos(mHandler, (int32*)&newPosX, (int32*)&newPosY);
        glfwGetWindowSize(mHandler, (int32*)&mWidth, (int32*)&mHeight);
        glfwGetFramebufferSize(mHandler, (int32*)&mFboWidth, (int32*)&mFboHeight);

        if (mOldWidth != mWidth || mOldHeight != mHeight) mIsSizeChanged = true;
        if (!mIsMovable && (newPosX != mPosX || newPosY != mPosY)) { glfwSetWindowPos(mHandler, mPosX, mPosY); }
        else { mPosX = newPosX; mPosY = newPosY; }
    }

    void GLWindow::getPosition(uint32 &x, uint32 &y)
    {
        x = mPosX;
        y = mPosY;
    }

    void GLWindow::getSize(uint32 &width, uint32 &height)
    {
        width = mWidth;
        height = mHeight;
    }

    void GLWindow::getFrameBufferSize(uint32 &width, uint32 &height)
    {
        width = mFboWidth;
        height = mFboHeight;
    }

    const bool GLWindow::shouldClose()
    {
        return (glfwWindowShouldClose(mHandler) != 0);
    }

    const bool GLWindow::getSizeChanged()
    {
        return mIsSizeChanged;
    }

    const char* GLWindow::getName()
    {
        return mWindowTitle.get();
    }

    const IWindow::WindowInfo& GLWindow::getWindowInfo()
    {
        WindowInfo result;

        result.width = mWidth;
        result.height = mHeight;

        result.posX = mPosX;
        result.posY = mPosY;

        result.fboWidth = mFboWidth;
        result.fboHeight = mFboHeight;

        result.isMovable = mIsMovable;
        result.iSFullScreen = mIsFullScreen;
        result.isResizable = mIsResizable;
        result.isSizeChanged = mIsSizeChanged;

        return result;
    }


} // namespace Berserk