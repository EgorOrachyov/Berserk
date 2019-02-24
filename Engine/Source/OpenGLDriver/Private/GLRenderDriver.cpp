//
// Created by Egor Orachyov on 07.02.2019.
//

#include "Misc/Assert.h"
#include "GLRenderDriver.h"
#include "Logging/LogMacros.h"

namespace Berserk
{

    void GLRenderDriver::initialize(const IWindow::WindowSetup &setup)
    {

        {
            // Setup glfw - Window and Input driver provider
            // In current implementation glfw will be initialized in the
            // Open GL Render device driver class

            if (!glfwInit())
            {
                FAIL(false, "Cannot initialize GLFW library");
            }

            #ifdef PLATFORM_MAC
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
            #else
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            #endif
        }


        {
            // Initialize main application window
            // Setup all common properties and pass info into gl window class

            auto handler = glfwCreateWindow(setup.width, setup.height, setup.caption.get(), nullptr, nullptr);

            glfwSetWindowPos(handler, setup.posX, setup.posY);

            if (setup.fullScreen)                      glfwMaximizeWindow(handler);
            if (!setup.resizable && !setup.fullScreen) glfwSetWindowSizeLimits(handler, setup.width, setup.height, setup.maxWidth, setup.maxHeight);

            glfwMakeContextCurrent(handler);

            // Setup GLWindow class
            mMainWindow.mHandler = handler;
            mMainWindow.initialize(setup);
        }


        {
            // Setup glew - OpenGL interface provider
            // Note: that no explicit close is needed

            if (glewInit() != GLEW_OK)
            {
                FAIL(false, "Cannot initialize GLEW library");
            }
        }

        PUSH("GLRenderDriver: initialize");
    }

    void GLRenderDriver::release()
    {
        mMainWindow.release();
        glfwTerminate();
        PUSH("GLRenderDriver: de-initialize");
    }

    void GLRenderDriver::clear(bool color, bool depth, bool stencil)
    {
        uint32 mask = 0;

        if (color)   mask |= GL_COLOR_BUFFER_BIT;
        if (depth)   mask |= GL_DEPTH_BUFFER_BIT;
        if (stencil) mask |= GL_STENCIL_BUFFER_BIT;

        glClear(mask);
    }

    void GLRenderDriver::setup(const RenderState &state)
    {

    }

    void GLRenderDriver::swapBuffers()
    {
        glfwSwapBuffers(mMainWindow.mHandler);
        glfwPollEvents();
    }

    void GLRenderDriver::setActive(IWindow *window)
    {
        glfwMakeContextCurrent((dynamic_cast<GLWindow*>(window))->mHandler);
    }

    IWindow * GLRenderDriver::getMainWindow()
    {
        return &mMainWindow;
    }

    const IRenderDriver::RenderState* GLRenderDriver::getCurrentState()
    {
        return nullptr;
    }

    const char* GLRenderDriver::getName()
    {

    }

    const char* GLRenderDriver::getInfo()
    {

    }

    const char* GLRenderDriver::getShaderInfo()
    {

    }

} // namespace Berserk