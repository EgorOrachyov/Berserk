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

#ifndef BERSERK_GLFWWINDOW_HPP
#define BERSERK_GLFWWINDOW_HPP

#include <platform/Window.hpp>

#include <GLFW/glfw3.h>

BRK_NS_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/**
 * @class GlfwWindow
 * @brief Glfw window implementation
 */
class GlfwWindow final : public Window {
public:
    BRK_API GlfwWindow(const StringName &name, const Size2i &size, String title, bool clientApi);
    BRK_API ~GlfwWindow() override;
    BRK_API Point2i GetPosition() const override;
    BRK_API Size2i GetSize() const override;
    BRK_API Size2i GetFramebufferSize() const override;
    BRK_API Vec2f GetPixelRatio() const override;
    BRK_API bool IsInFocus() const override;
    BRK_API bool IsClosed() const override;
    BRK_API String GetTitle() const override;
    BRK_API StringName GetName() const override;

private:
    friend class GlfwWindowManager;

    Point2i mPosition;
    Size2i mSize;
    Size2i mFramebufferSize;
    Vec2f mPixelRatio;
    String mTitle;
    StringName mName;
    bool mInFocus = false;

    GLFWwindow *mHND = nullptr;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_GLFWWINDOW_HPP
