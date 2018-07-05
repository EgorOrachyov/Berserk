//
// Created by Egor Orachyov on 04.07.2018.
//

#include "GLWindow.h"

#include "../../Core/Essential/Assert.h"
#include "../../Core/Logging/LogMessages.h"

namespace Berserk
{

    GLWindow::GLWindow()
    {
        mWindow = NULL;
        mWidth = 0;
        mHeight = 0;
        mIsCreated = false;
    }

    GLWindow::~GLWindow()
    {
        destroy();
    }

    void GLWindow::create()
    {
        mWidth = 800;
        mHeight = 600;
        const CHAR name[] = "Default window";

        mWindow = glfwCreateWindow(mWidth, mHeight, name, NULL, NULL);
        ASSERT(mWindow, "Cannot create GL window with name %s", name);
        if (!mWindow)
        {
            ERROR("Cannot create GL window with name %s", name);
            mIsCreated = false;
            return;
        }

        mIsCreated = true;
    }

    void GLWindow::create(uint32 width, uint32 height, const CHAR* name)
    {
        mWidth = width;
        mHeight = height;

        mWindow = glfwCreateWindow(mWidth, mHeight, name, NULL, NULL);
        ASSERT(mWindow, "Cannot create GL window with name %s", name);
        if (!mWindow)
        {
            ERROR("Cannot create GL window with name %s", name);
            mIsCreated = false;
            return;
        }

        mIsCreated = true;
    }

    void GLWindow::destroy()
    {
        if (mIsCreated)
        {
            glfwDestroyWindow(mWindow);
        }
        mWindow = NULL;
        mWidth = 0;
        mHeight = 0;
        mIsCreated = false;
    }

    void GLWindow::makeCurrent() const
    {
        glfwMakeContextCurrent(mWindow);
    }

    GLFWwindow* GLWindow::getHandle() const
    {
        return mWindow;
    }

    uint32 GLWindow::getWidth() const
    {
        return mWidth;
    }

    uint32 GLWindow::getHeight() const
    {
        return mHeight;
    }

    bool GLWindow::isCreated() const
    {
        return mIsCreated;
    }


} // namespace Berserk