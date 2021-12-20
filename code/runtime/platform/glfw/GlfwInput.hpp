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

#ifndef BERSERK_GLFWINPUT_HPP
#define BERSERK_GLFWINPUT_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <platform/Input.hpp>
#include <platform/glfw/GlfwInputDevices.hpp>

#include <GLFW/glfw3.h>
#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/**
 * @class GlfwInput
 * @brief Glfw input manager implementation
 */
class GlfwInput final : public Input {
public:
    BRK_API GlfwInput();
    BRK_API ~GlfwInput() override = default;
    BRK_API Ref<Mouse> GetMouse() override;
    BRK_API Ref<Keyboard> GetKeyboard() override;
    BRK_API Ref<Joystick> GetJoystick() override;

private:
    friend class GlfwWindowManager;
    void SubscribeWindow(GLFWwindow *window);
    void PreUpdate();
    void PostUpdate();
    void CheckConnectedJoysticks();
    Ref<GlfwJoystick> GetJoystick(int32 jid);

private:
    // Glfw specific
    static void DropCallback(GLFWwindow *window, int32 count, const char **paths);
    static void MousePositionCallback(GLFWwindow *window, double x, double y);
    static void MouseButtonsCallback(GLFWwindow *window, int32 button, int32 action, int32 mods);
    static void KeyboardKeysCallback(GLFWwindow *window, int32 key, int32 scancode, int32 action, int32 mods);
    static void KeyboardTextCallback(GLFWwindow *window, unsigned int codePoint);
    static void JoystickCallback(int32 jid, int32 state);

private:
    Ref<GlfwMouse> mMouse;
    Ref<GlfwKeyboard> mKeyboard;
    std::vector<Ref<GlfwJoystick>> mJoysticks;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_GLFWINPUT_HPP
