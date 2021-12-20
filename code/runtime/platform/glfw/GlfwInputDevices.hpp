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

#ifndef BERSERK_GLFWINPUTDEVICES_HPP
#define BERSERK_GLFWINPUTDEVICES_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <platform/InputDevices.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/**
 * @class GlfwMouse
 * @brief Glfw device implementation
 */
class GlfwMouse final : public Mouse {
public:
    BRK_API GlfwMouse();
    BRK_API ~GlfwMouse() override = default;

    void UpdatePosition(const Point2f &pos);
    void UpdateButton(InputMouseButton button, InputAction action);
};

/**
 * @class GlfwKeyboard
 * @brief Glfw device implementation
 */
class GlfwKeyboard final : public Keyboard {
public:
    BRK_API GlfwKeyboard();
    BRK_API ~GlfwKeyboard() override = default;

    void UpdateKey(InputKeyboardKey key, InputAction action);
};

/**
 * @class GlfwJoystick
 * @brief Glfw device implementation
 */
class GlfwJoystick final : public Joystick {
public:
    BRK_API explicit GlfwJoystick(int32 HND);
    BRK_API ~GlfwJoystick() override = default;

    void Update();
    void UpdateState(InputDeviceState state);
    int32 GetHND() const { return mHND; }

private:
    int32 mHND = -1;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_GLFWINPUTDEVICES_HPP
