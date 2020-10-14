/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_GLFWDEFINITIONS_HPP
#define BERSERK_GLFWDEFINITIONS_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/Platform/InputDefinitions.hpp>
#include <BerserkCore/TMask.hpp>
#include <GLFW/glfw3.h>

namespace Berserk {

    class GlfwDefinitions final{
    public:

        static TMask<EModifier> GetModsMask(int32 mods) {
            auto umods = (uint32) mods;

            const EModifier MODS[] = { EModifier::Shift, EModifier::Alt, EModifier::Control, EModifier::CapsLock, EModifier::NumLock };
            const uint32 GLFW_MODS[] = { GLFW_MOD_ALT, GLFW_MOD_CAPS_LOCK, GLFW_MOD_CONTROL, GLFW_MOD_NUM_LOCK, GLFW_MOD_SHIFT };

            TMask<EModifier> mask;

            for (uint32 i = 0; i < sizeof(MODS)/sizeof(MODS[0]); i++) {
                if ((umods & GLFW_MODS[i]) != 0) {
                    mask = mask.AddBit(MODS[i]);
                }
            }

            return mask;
        }

        static EMouseButton GetMouseButton(int32 button) {
            switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT:
                    return EMouseButton::Left;
                case GLFW_MOUSE_BUTTON_RIGHT:
                    return EMouseButton::Right;
                default:
                    return EMouseButton::MarkerUnknown;
            }
        }

        static EInputAction GetAction(int32 action) {
            switch (action) {
                case GLFW_PRESS:
                    return EInputAction::Press;
                case GLFW_RELEASE:
                    return EInputAction::Release;
                default:
                    return EInputAction::MarkerUnknown;
            }
        }

        static EKeyboardKey GetKeyboardKey(int32 key) {
            switch (key) {
                case GLFW_KEY_SPACE:
                    return EKeyboardKey::Space;
                case GLFW_KEY_APOSTROPHE:
                    return EKeyboardKey::Apostrophe;
                case GLFW_KEY_COMMA:
                    return EKeyboardKey::Comma;
                case GLFW_KEY_MINUS:
                    return EKeyboardKey::Minus;
                case GLFW_KEY_PERIOD:
                    return EKeyboardKey::Period;
                case GLFW_KEY_SLASH:
                    return EKeyboardKey::Slash;
                case GLFW_KEY_BACKSLASH:
                    return EKeyboardKey::BackSlash;
                case GLFW_KEY_SEMICOLON:
                    return EKeyboardKey::Semicolon;
                case GLFW_KEY_EQUAL:
                    return EKeyboardKey::Equal;
                case GLFW_KEY_LEFT_BRACKET:
                    return EKeyboardKey::LeftBracket;
                case GLFW_KEY_RIGHT_BRACKET:
                    return EKeyboardKey::RightBracket;

                case GLFW_KEY_0:
                    return EKeyboardKey::Num0;
                case GLFW_KEY_1:
                    return EKeyboardKey::Num1;
                case GLFW_KEY_2:
                    return EKeyboardKey::Num2;
                case GLFW_KEY_3:
                    return EKeyboardKey::Num3;
                case GLFW_KEY_4:
                    return EKeyboardKey::Num4;
                case GLFW_KEY_5:
                    return EKeyboardKey::Num5;
                case GLFW_KEY_6:
                    return EKeyboardKey::Num6;
                case GLFW_KEY_7:
                    return EKeyboardKey::Num7;
                case GLFW_KEY_8:
                    return EKeyboardKey::Num8;
                case GLFW_KEY_9:
                    return EKeyboardKey::Num9;

                case GLFW_KEY_A:
                    return EKeyboardKey::A;
                case GLFW_KEY_B:
                    return EKeyboardKey::B;
                case GLFW_KEY_C:
                    return EKeyboardKey::C;
                case GLFW_KEY_D:
                    return EKeyboardKey::D;
                case GLFW_KEY_E:
                    return EKeyboardKey::E;
                case GLFW_KEY_F:
                    return EKeyboardKey::F;
                case GLFW_KEY_G:
                    return EKeyboardKey::G;
                case GLFW_KEY_H:
                    return EKeyboardKey::H;
                case GLFW_KEY_I:
                    return EKeyboardKey::I;
                case GLFW_KEY_J:
                    return EKeyboardKey::J;
                case GLFW_KEY_K:
                    return EKeyboardKey::K;
                case GLFW_KEY_L:
                    return EKeyboardKey::L;
                case GLFW_KEY_M:
                    return EKeyboardKey::M;
                case GLFW_KEY_N:
                    return EKeyboardKey::N;
                case GLFW_KEY_O:
                    return EKeyboardKey::O;
                case GLFW_KEY_P:
                    return EKeyboardKey::P;
                case GLFW_KEY_Q:
                    return EKeyboardKey::Q;
                case GLFW_KEY_R:
                    return EKeyboardKey::R;
                case GLFW_KEY_S:
                    return EKeyboardKey::S;
                case GLFW_KEY_T:
                    return EKeyboardKey::T;
                case GLFW_KEY_U:
                    return EKeyboardKey::U;
                case GLFW_KEY_V:
                    return EKeyboardKey::V;
                case GLFW_KEY_W:
                    return EKeyboardKey::W;
                case GLFW_KEY_X:
                    return EKeyboardKey::X;
                case GLFW_KEY_Y:
                    return EKeyboardKey::Y;
                case GLFW_KEY_Z:
                    return EKeyboardKey::Z;

                case GLFW_KEY_F1:
                    return EKeyboardKey::F1;
                case GLFW_KEY_F2:
                    return EKeyboardKey::F2;
                case GLFW_KEY_F3:
                    return EKeyboardKey::F3;
                case GLFW_KEY_F4:
                    return EKeyboardKey::F4;
                case GLFW_KEY_F5:
                    return EKeyboardKey::F5;
                case GLFW_KEY_F6:
                    return EKeyboardKey::F6;
                case GLFW_KEY_F7:
                    return EKeyboardKey::F7;
                case GLFW_KEY_F8:
                    return EKeyboardKey::F8;
                case GLFW_KEY_F9:
                    return EKeyboardKey::F9;
                case GLFW_KEY_F10:
                    return EKeyboardKey::F10;
                case GLFW_KEY_F11:
                    return EKeyboardKey::F11;
                case GLFW_KEY_F12:
                    return EKeyboardKey::F12;

                case GLFW_KEY_ESCAPE:
                    return EKeyboardKey::Escape;
                case GLFW_KEY_ENTER:
                    return EKeyboardKey::Enter;
                case GLFW_KEY_TAB:
                    return EKeyboardKey::Tab;
                case GLFW_KEY_BACKSPACE:
                    return EKeyboardKey::Backspace;
                case GLFW_KEY_INSERT:
                    return EKeyboardKey::Insert;
                case GLFW_KEY_DELETE:
                    return EKeyboardKey::Delete;
                case GLFW_KEY_RIGHT:
                    return EKeyboardKey::Right;
                case GLFW_KEY_LEFT:
                    return EKeyboardKey::Left;
                case GLFW_KEY_DOWN:
                    return EKeyboardKey::Down;
                case GLFW_KEY_UP:
                    return EKeyboardKey::Up;
                case GLFW_KEY_PAGE_UP:
                    return EKeyboardKey::PageUp;
                case GLFW_KEY_PAGE_DOWN:
                    return EKeyboardKey::PageDown;

                case GLFW_KEY_CAPS_LOCK:
                    return EKeyboardKey::CapsLock;
                case GLFW_KEY_SCROLL_LOCK:
                    return EKeyboardKey::ScrollLock;
                case GLFW_KEY_NUM_LOCK:
                    return EKeyboardKey::NumLock;
                case GLFW_KEY_PRINT_SCREEN:
                    return EKeyboardKey::PrintScreen;
                case GLFW_KEY_PAUSE:
                    return EKeyboardKey::Pause;

                case GLFW_KEY_LEFT_SHIFT:
                    return EKeyboardKey::LeftShift;
                case GLFW_KEY_LEFT_CONTROL:
                    return EKeyboardKey::LeftControl;
                case GLFW_KEY_LEFT_ALT:
                    return EKeyboardKey::LeftAlt;
                case GLFW_KEY_LEFT_SUPER:
                    return EKeyboardKey::LeftSuper;
                case GLFW_KEY_RIGHT_SHIFT:
                    return EKeyboardKey::RightShift;
                case GLFW_KEY_RIGHT_CONTROL:
                    return EKeyboardKey::RightControl;
                case GLFW_KEY_RIGHT_ALT:
                    return EKeyboardKey::RightAlt;
                case GLFW_KEY_RIGHT_SUPER:
                    return EKeyboardKey::RightSuper;

                case GLFW_KEY_HOME:
                    return EKeyboardKey::Home;
                case GLFW_KEY_END:
                    return EKeyboardKey::End;
                case GLFW_KEY_MENU:
                    return EKeyboardKey::Menu;

                default:
                    return EKeyboardKey::MarkerUnknown;
            }
        }

    };

}

#endif //BERSERK_GLFWDEFINITIONS_HPP
