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

#ifndef BERSERK_GLFWINPUTDEFS_HPP
#define BERSERK_GLFWINPUTDEFS_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <platform/InputDefs.hpp>

#include <GLFW/glfw3.h>

BRK_NS_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/**
 * @class GlfwInputDefs
 * @brief Glfw input defines
 */
class GlfwInputDefs final {
public:
    static InputModifiers GetModsMask(int32 mods) {
        InputModifiers mask;
        auto umods = static_cast<uint32>(mods);

        mask.Set(InputModifier::Alt, umods & (uint32) GLFW_MOD_ALT);
        mask.Set(InputModifier::CapsLock, umods & (uint32) GLFW_MOD_CAPS_LOCK);
        mask.Set(InputModifier::Control, umods & (uint32) GLFW_MOD_CONTROL);
        mask.Set(InputModifier::NumLock, umods & (uint32) GLFW_MOD_NUM_LOCK);
        mask.Set(InputModifier::Shift, umods & (uint32) GLFW_MOD_SHIFT);

        return mask;
    }

    static InputMouseButton GetMouseButton(int32 button) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                return InputMouseButton::Left;
            case GLFW_MOUSE_BUTTON_RIGHT:
                return InputMouseButton::Right;
            default:
                return InputMouseButton::Unknown;
        }
    }

    static InputAction GetAction(int32 action) {
        switch (action) {
            case GLFW_PRESS:
                return InputAction::Press;
            case GLFW_RELEASE:
                return InputAction::Release;
            default:
                return InputAction::Unknown;
        }
    }

    static InputKeyboardKey GetKeyboardKey(int32 key) {
        switch (key) {
            case GLFW_KEY_SPACE:
                return InputKeyboardKey::Space;
            case GLFW_KEY_APOSTROPHE:
                return InputKeyboardKey::Apostrophe;
            case GLFW_KEY_COMMA:
                return InputKeyboardKey::Comma;
            case GLFW_KEY_MINUS:
                return InputKeyboardKey::Minus;
            case GLFW_KEY_PERIOD:
                return InputKeyboardKey::Period;
            case GLFW_KEY_SLASH:
                return InputKeyboardKey::Slash;
            case GLFW_KEY_BACKSLASH:
                return InputKeyboardKey::BackSlash;
            case GLFW_KEY_SEMICOLON:
                return InputKeyboardKey::Semicolon;
            case GLFW_KEY_EQUAL:
                return InputKeyboardKey::Equal;
            case GLFW_KEY_LEFT_BRACKET:
                return InputKeyboardKey::LeftBracket;
            case GLFW_KEY_RIGHT_BRACKET:
                return InputKeyboardKey::RightBracket;

            case GLFW_KEY_0:
                return InputKeyboardKey::Num0;
            case GLFW_KEY_1:
                return InputKeyboardKey::Num1;
            case GLFW_KEY_2:
                return InputKeyboardKey::Num2;
            case GLFW_KEY_3:
                return InputKeyboardKey::Num3;
            case GLFW_KEY_4:
                return InputKeyboardKey::Num4;
            case GLFW_KEY_5:
                return InputKeyboardKey::Num5;
            case GLFW_KEY_6:
                return InputKeyboardKey::Num6;
            case GLFW_KEY_7:
                return InputKeyboardKey::Num7;
            case GLFW_KEY_8:
                return InputKeyboardKey::Num8;
            case GLFW_KEY_9:
                return InputKeyboardKey::Num9;

            case GLFW_KEY_A:
                return InputKeyboardKey::A;
            case GLFW_KEY_B:
                return InputKeyboardKey::B;
            case GLFW_KEY_C:
                return InputKeyboardKey::C;
            case GLFW_KEY_D:
                return InputKeyboardKey::D;
            case GLFW_KEY_E:
                return InputKeyboardKey::E;
            case GLFW_KEY_F:
                return InputKeyboardKey::F;
            case GLFW_KEY_G:
                return InputKeyboardKey::G;
            case GLFW_KEY_H:
                return InputKeyboardKey::H;
            case GLFW_KEY_I:
                return InputKeyboardKey::I;
            case GLFW_KEY_J:
                return InputKeyboardKey::J;
            case GLFW_KEY_K:
                return InputKeyboardKey::K;
            case GLFW_KEY_L:
                return InputKeyboardKey::L;
            case GLFW_KEY_M:
                return InputKeyboardKey::M;
            case GLFW_KEY_N:
                return InputKeyboardKey::N;
            case GLFW_KEY_O:
                return InputKeyboardKey::O;
            case GLFW_KEY_P:
                return InputKeyboardKey::P;
            case GLFW_KEY_Q:
                return InputKeyboardKey::Q;
            case GLFW_KEY_R:
                return InputKeyboardKey::R;
            case GLFW_KEY_S:
                return InputKeyboardKey::S;
            case GLFW_KEY_T:
                return InputKeyboardKey::T;
            case GLFW_KEY_U:
                return InputKeyboardKey::U;
            case GLFW_KEY_V:
                return InputKeyboardKey::V;
            case GLFW_KEY_W:
                return InputKeyboardKey::W;
            case GLFW_KEY_X:
                return InputKeyboardKey::X;
            case GLFW_KEY_Y:
                return InputKeyboardKey::Y;
            case GLFW_KEY_Z:
                return InputKeyboardKey::Z;

            case GLFW_KEY_F1:
                return InputKeyboardKey::F1;
            case GLFW_KEY_F2:
                return InputKeyboardKey::F2;
            case GLFW_KEY_F3:
                return InputKeyboardKey::F3;
            case GLFW_KEY_F4:
                return InputKeyboardKey::F4;
            case GLFW_KEY_F5:
                return InputKeyboardKey::F5;
            case GLFW_KEY_F6:
                return InputKeyboardKey::F6;
            case GLFW_KEY_F7:
                return InputKeyboardKey::F7;
            case GLFW_KEY_F8:
                return InputKeyboardKey::F8;
            case GLFW_KEY_F9:
                return InputKeyboardKey::F9;
            case GLFW_KEY_F10:
                return InputKeyboardKey::F10;
            case GLFW_KEY_F11:
                return InputKeyboardKey::F11;
            case GLFW_KEY_F12:
                return InputKeyboardKey::F12;

            case GLFW_KEY_ESCAPE:
                return InputKeyboardKey::Escape;
            case GLFW_KEY_ENTER:
                return InputKeyboardKey::Enter;
            case GLFW_KEY_TAB:
                return InputKeyboardKey::Tab;
            case GLFW_KEY_BACKSPACE:
                return InputKeyboardKey::Backspace;
            case GLFW_KEY_INSERT:
                return InputKeyboardKey::Insert;
            case GLFW_KEY_DELETE:
                return InputKeyboardKey::Delete;
            case GLFW_KEY_RIGHT:
                return InputKeyboardKey::Right;
            case GLFW_KEY_LEFT:
                return InputKeyboardKey::Left;
            case GLFW_KEY_DOWN:
                return InputKeyboardKey::Down;
            case GLFW_KEY_UP:
                return InputKeyboardKey::Up;
            case GLFW_KEY_PAGE_UP:
                return InputKeyboardKey::PageUp;
            case GLFW_KEY_PAGE_DOWN:
                return InputKeyboardKey::PageDown;

            case GLFW_KEY_CAPS_LOCK:
                return InputKeyboardKey::CapsLock;
            case GLFW_KEY_SCROLL_LOCK:
                return InputKeyboardKey::ScrollLock;
            case GLFW_KEY_NUM_LOCK:
                return InputKeyboardKey::NumLock;
            case GLFW_KEY_PRINT_SCREEN:
                return InputKeyboardKey::PrintScreen;
            case GLFW_KEY_PAUSE:
                return InputKeyboardKey::Pause;

            case GLFW_KEY_LEFT_SHIFT:
                return InputKeyboardKey::LeftShift;
            case GLFW_KEY_LEFT_CONTROL:
                return InputKeyboardKey::LeftControl;
            case GLFW_KEY_LEFT_ALT:
                return InputKeyboardKey::LeftAlt;
            case GLFW_KEY_LEFT_SUPER:
                return InputKeyboardKey::LeftSuper;
            case GLFW_KEY_RIGHT_SHIFT:
                return InputKeyboardKey::RightShift;
            case GLFW_KEY_RIGHT_CONTROL:
                return InputKeyboardKey::RightControl;
            case GLFW_KEY_RIGHT_ALT:
                return InputKeyboardKey::RightAlt;
            case GLFW_KEY_RIGHT_SUPER:
                return InputKeyboardKey::RightSuper;

            case GLFW_KEY_HOME:
                return InputKeyboardKey::Home;
            case GLFW_KEY_END:
                return InputKeyboardKey::End;
            case GLFW_KEY_MENU:
                return InputKeyboardKey::Menu;

            default:
                return InputKeyboardKey::Unknown;
        }
    }
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_GLFWINPUTDEFS_HPP
