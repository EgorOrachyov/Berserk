//
// Created by Egor Orachyov on 21.02.2019.
//

#ifndef BERSERK_GLWINDOW_H
#define BERSERK_GLWINDOW_H

#include "Platform/IWindow.h"
#include "GLFW/glfw3.h"

namespace Berserk
{

    class GLWindow : public IWindow
    {
    private:

        friend class GLRenderDriver;

        GLFWwindow* mHandler;

    };

} // namespace Berserk

#endif //BERSERK_GLWINDOW_H