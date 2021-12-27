/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <platform/glfw/GlfwWindow.hpp>
#include <utility>

BRK_NS_BEGIN

GlfwWindow::GlfwWindow(StringName name, const Size2i &size, String title, bool clientApi) : mName(std::move(name)), mTitle(std::move(title)) {
    if (clientApi) {
#if defined(BERSERK_TARGET_MACOS)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#elif defined(BERSERK_TARGET_LINUX)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
#elif defined(BERSERK_TARGET_WINDOWS)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif
    }

    mHND = glfwCreateWindow(size.x(), size.y(), mTitle.c_str(), nullptr, nullptr);

    glfwGetWindowPos(mHND, &mPosition[0], &mPosition[1]);
    glfwGetWindowSize(mHND, &mSize[0], &mSize[1]);
    glfwGetFramebufferSize(mHND, &mFramebufferSize[0], &mFramebufferSize[1]);
    glfwGetWindowContentScale(mHND, &mPixelRatio[0], &mPixelRatio[1]);

    mInFocus = glfwGetWindowAttrib(mHND, GLFW_FOCUSED);
}

GlfwWindow::~GlfwWindow() {
    if (mHND) {
        glfwDestroyWindow(mHND);
        mHND = nullptr;
    }
}

Point2i GlfwWindow::GetPosition() const {
    return mPosition;
}

Size2i GlfwWindow::GetSize() const {
    return mSize;
}

Size2i GlfwWindow::GetFramebufferSize() const {
    return mFramebufferSize;
}

Vec2f GlfwWindow::GetPixelRatio() const {
    return mPixelRatio;
}

bool GlfwWindow::IsInFocus() const {
    return mInFocus;
}

bool GlfwWindow::IsClosed() const {
    return false;
}

String GlfwWindow::GetTitle() const {
    return mTitle;
}

StringName GlfwWindow::GetName() const {
    return mName;
}

BRK_NS_END
