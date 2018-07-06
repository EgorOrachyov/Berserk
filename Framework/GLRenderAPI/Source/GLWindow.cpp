//
// Created by Egor Orachyov on 04.07.2018.
//

#include "Context/GLWindow.h"

#include "../../Core/Essential/Assert.h"
#include "../../Core/Logging/LogMessages.h"

namespace Berserk
{

    GLWindow::GLWindow()
    {
        mHandle = NULL;
        mWidth = 0;
        mHeight = 0;
    }

    GLWindow::~GLWindow()
    {
        destroy();
    }

    void GLWindow::create()
    {
        uint32 width = 500;   // Default width
        uint32 height = 500;  // Default size (for debugging)
        const CHAR name[] = "Default window";

        mHandle = glfwCreateWindow(width, height, name, NULL, NULL);
        ASSERT(mHandle, "Cannot create GL window with name %s", name);
        if (!mHandle)
        {
            ERROR("Cannot create GL window with name %s", name);
            mHandle = NULL;
            return;
        }
        
        mWidth = width;
        mHeight = height;
    }

    void GLWindow::create(uint32 width, uint32 height, const CHAR* name)
    {
        mHandle = glfwCreateWindow(width, height, name, NULL, NULL);
        ASSERT(mHandle, "Cannot create GL window with name %s", name);
        if (!mHandle)
        {
            ERROR("Cannot create GL window with name %s", name);
            mHandle = NULL;
            return;
        }
        
        mWidth = width;
        mHeight = height;
    }

    void GLWindow::destroy()
    {
        if (mHandle != NULL)
        {
            glfwDestroyWindow(mHandle);
        }
        mHandle = NULL;
        mWidth = 0;
        mHeight = 0;
    }

    void GLWindow::makeCurrent() const
    {
        glfwMakeContextCurrent(mHandle);
    }

    GLFWwindow* GLWindow::getHandle() const
    {
        return mHandle;
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
        return (mHandle != NULL);
    }


} // namespace Berserk
