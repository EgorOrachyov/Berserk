//
// Created by Egor Orachyov on 07.02.2019.
//

#include "GLRenderDriver.h"
#include "Logging/LogMacros.h"

namespace Berserk
{

    void GLRenderDriver::initialize(uint32 width, uint32 height, const char *caption)
    {
        if (!glfwInit())
        {
            ERROR("Cannot initialize GLFW library");
            return;
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

        mMainWindow.mHandler = glfwCreateWindow(width, height, caption, nullptr, nullptr);

        glfwMakeContextCurrent(mMainWindow.mHandler);

        if (glewInit() != GLEW_OK)
        {
            ERROR("Cannot initialize GLEW library");
            return;
        }
    }

    void GLRenderDriver::release()
    {
        glfwDestroyWindow(mMainWindow.mHandler);
        glfwTerminate();
    }

    void GLRenderDriver::clear(bool color, bool depth, bool stencil)
    {

    }

    void GLRenderDriver::setup(const RenderState &state)
    {

    }

    void GLRenderDriver::swapBuffers()
    {
        glfwSwapBuffers(mMainWindow.mHandler);
    }

    void GLRenderDriver::setActive(IWindow *window)
    {
        glfwMakeContextCurrent((dynamic_cast<GLWindow*>(window))->mHandler);
    }

    const IWindow* GLRenderDriver::getMainWindow()
    {
        return &mMainWindow;
    }

    const IRenderDriver::RenderState* GLRenderDriver::getCurrentState()
    {
        return &mRenderState;
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