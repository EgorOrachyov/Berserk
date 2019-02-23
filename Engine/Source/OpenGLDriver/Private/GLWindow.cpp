//
// Created by Egor Orachyov on 21.02.2019.
//

#include "GLWindow.h"
#include "Logging/LogMacros.h"

namespace Berserk
{

    void GLWindow::initialize(const WindowSetup &info)
    {
        mWindowTitle = info.caption;
        PUSH("GLWindow: initialize window [name: %s]", mWindowTitle.get());
    }

    void GLWindow::release()
    {
        glfwDestroyWindow(mHandler);
        PUSH("GLWindow: destroy window [name: %s]", mWindowTitle.get());
    }

    void GLWindow::setPosition(uint32 x, uint32 y)
    {

    }

    void GLWindow::setSize(uint32 width, uint32 height)
    {

    }

    void GLWindow::setMovable(bool flag)
    {

    }

    void GLWindow::setResizable(bool flag)
    {

    }

    void GLWindow::update()
    {

    }

    void GLWindow::getPosition(uint32 &x, uint32 &y)
    {

    }

    void GLWindow::getSize(uint32 &width, uint32 &height)
    {

    }

    void GLWindow::getFrameBufferSize(uint32 &width, uint32 &height)
    {

    }

    const bool GLWindow::shouldClose()
    {
        return (glfwWindowShouldClose(mHandler) != 0);
    }

    const bool GLWindow::getSizeChanged()
    {

    }

    const char* GLWindow::getName()
    {

    }

    const IWindow::WindowInfo& GLWindow::getWindowInfo()
    {

    }


} // namespace Berserk