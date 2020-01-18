/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLFWINPUT_H
#define BERSERK_GLFWINPUT_H

#include <Platform/Input.h>
#include <Errors.h>
#include <GLFW/glfw3.h>

namespace Berserk {

    class GlfwInput : public Input {
    public:

        static const uint32 INPUT_STATES_COUNT = 2;
        static const uint32 MOUSE_BUTTONS_COUNT = 2;
        static const uint32 KEYBOARD_KEYS_COUNT = 100;
        static const uint32 UNDEFINED_ELEMENT = 0xffffffff;

        struct InputState {
            InputState() {
                for (auto& action: mouseButtons) {
                    action = EInputAction::Nothing;
                }

                for (auto& action: keyboardKeys) {
                    action = EInputAction::Nothing;
                }
            }

            void resetTmpData() {
                for (auto& action: mouseButtons) {
                    action = EInputAction::Nothing;
                }

                for (auto& action: keyboardKeys) {
                    action = EInputAction::Nothing;
                }

                mouseDelta.zero();
                modifiersMask = 0x0;
            }

            EModifiersMask modifiersMask = 0x0;
            Point2i        mousePosition;
            Point2i        mouseDelta;
            EInputAction   mouseButtons[MOUSE_BUTTONS_COUNT] = {};
            EInputAction   keyboardKeys[KEYBOARD_KEYS_COUNT] = {};
        };

        GlfwInput() noexcept : Input() {}
        ~GlfwInput() override = default;

        /**
         * Initializie actual input system with input events handling.
         * Called after primary application window created.
         * @param primaryWindow Main application window to track input
         */
        void initialize(GLFWwindow* primaryWindow) {
            mWindowHandle = primaryWindow;
            glfwSetCursorPosCallback(mWindowHandle, mousePositionCallback);
            glfwSetMouseButtonCallback(mWindowHandle, mouseButtonsCallback);
            glfwSetKeyCallback(mWindowHandle, keyboardKeysCallback);
        }

        Point2i getMousePosition() const override {
            return mStates[mReadIndex].mousePosition;
        }
        Point2i getMouseDelta() const override {
            return mStates[mReadIndex].mouseDelta;
        }
        bool isButtonPressed(EMouseButton button) const override {
            return mStates[mReadIndex].mouseButtons[(uint32)button] == EInputAction::Press;
        }
        bool isButtonReleased(EMouseButton button) const override {
            return mStates[mReadIndex].mouseButtons[(uint32)button] == EInputAction::Release;
        }

        EModifiersMask getModifiersMask() const override {
            return mStates[mReadIndex].modifiersMask;
        }

        bool isKeyPressed(EKeyboardKey key) const override {
            return mStates[mReadIndex].keyboardKeys[(uint32)key] == EInputAction::Press;
        }
        bool isKeyReleased(EKeyboardKey key) const override {
            return mStates[mReadIndex].keyboardKeys[(uint32)key] == EInputAction::Release;
        }
        bool isKeyRepeated(EKeyboardKey key) const override {
            return mStates[mReadIndex].keyboardKeys[(uint32)key] == EInputAction::Repeat;
        }

        /**
         * Change indices of the current state (read / write).
         * All the update is done via glfwPollEvents() and static callback members
         */
        void update() {
            // Note: here glfwPollEvents() already called
            mWriteIndex = mReadIndex;
            mReadIndex = (mReadIndex + 1) % INPUT_STATES_COUNT;

            auto& write = mStates[mWriteIndex];
            write.resetTmpData();
        }

        static void mousePositionCallback(GLFWwindow* window, float64 x, float64 y) {
            auto& write = mStates[mWriteIndex];
            auto& read = mStates[mReadIndex];

            write.mousePosition = Point2i(static_cast<int32>(x), static_cast<int32>(y));
            write.mouseDelta = write.mousePosition - read.mousePosition;
        }

        static void mouseButtonsCallback(GLFWwindow* window, int32 button, int32 action, int32 mods) {
            auto& write = mStates[mWriteIndex];

            write.modifiersMask = getModsMask(mods);
            auto mouseButton = getMouseButton(button);

            if (mouseButton != UNDEFINED_ELEMENT) {
                auto mouseButtonAction = getAction(action);
                write.mouseButtons[mouseButton] = mouseButtonAction;
            }
        }

        static void keyboardKeysCallback(GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods)
        {
            auto& write = mStates[mWriteIndex];
            auto& read = mStates[mReadIndex];

            write.modifiersMask = getModsMask(mods);
            auto keyboardKey = getKeyboardKey(key);

            if (keyboardKey != UNDEFINED_ELEMENT) {
                auto keyboardKeyAction = getAction(action);
                write.keyboardKeys[keyboardKey] = keyboardKeyAction;
            }
        }

        static EModifiersMask getModsMask(int32 mods) {
            EModifiersMask mask = 0x0;

            if (mods & GLFW_MOD_ALT)
                mask |= (uint32) EModifierMask::Alt;
            if (mods & GLFW_MOD_CAPS_LOCK)
                mask |= (uint32) EModifierMask::CapsLock;
            if (mods & GLFW_MOD_CONTROL)
                mask |= (uint32) EModifierMask::CapsLock;
            if (mods & GLFW_MOD_NUM_LOCK)
                mask |= (uint32) EModifierMask::NumLock;
            if (mods & GLFW_MOD_SHIFT)
                mask |= (uint32) EModifierMask::Shift;

            return mask;
        }

        static uint32 getMouseButton(int32 button) {
            switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT:
                    return (uint32) EMouseButton::Left;
                case GLFW_MOUSE_BUTTON_RIGHT:
                    return (uint32) EMouseButton::Right;
                default:
                    return UNDEFINED_ELEMENT;
            }
        }

        static EInputAction getAction(int32 action) {
            switch (action) {
                case GLFW_PRESS:
                    return EInputAction::Press;
                case GLFW_RELEASE:
                    return EInputAction::Release;
                case GLFW_REPEAT:
                    return EInputAction::Repeat;
                default:
                    return EInputAction::Nothing;
            }
        }

        static uint32 getKeyboardKey(int32 key) {
            switch (key) {
                case GLFW_KEY_SPACE:
                    return (uint32)EKeyboardKey::Space;
                case GLFW_KEY_APOSTROPHE:
                    return (uint32)EKeyboardKey::Apostrophe;
                case GLFW_KEY_COMMA:
                    return (uint32)EKeyboardKey::Comma;
                case GLFW_KEY_MINUS:
                    return (uint32)EKeyboardKey::Minus;
                case GLFW_KEY_PERIOD:
                    return (uint32)EKeyboardKey::Period;
                case GLFW_KEY_SLASH:
                    return (uint32)EKeyboardKey::Slash;
                case GLFW_KEY_BACKSLASH:
                    return (uint32)EKeyboardKey::BackSlash;
                case GLFW_KEY_SEMICOLON:
                    return (uint32)EKeyboardKey::Semicolon;
                case GLFW_KEY_EQUAL:
                    return (uint32)EKeyboardKey::Equal;
                case GLFW_KEY_LEFT_BRACKET:
                    return (uint32)EKeyboardKey::LeftBracket;
                case GLFW_KEY_RIGHT_BRACKET:
                    return (uint32)EKeyboardKey::RightBracket;

                case GLFW_KEY_0:
                    return (uint32)EKeyboardKey::Num0;
                case GLFW_KEY_1:
                    return (uint32)EKeyboardKey::Num1;
                case GLFW_KEY_2:
                    return (uint32)EKeyboardKey::Num2;
                case GLFW_KEY_3:
                    return (uint32)EKeyboardKey::Num3;
                case GLFW_KEY_4:
                    return (uint32)EKeyboardKey::Num4;
                case GLFW_KEY_5:
                    return (uint32)EKeyboardKey::Num5;
                case GLFW_KEY_6:
                    return (uint32)EKeyboardKey::Num6;
                case GLFW_KEY_7:
                    return (uint32)EKeyboardKey::Num7;
                case GLFW_KEY_8:
                    return (uint32)EKeyboardKey::Num8;
                case GLFW_KEY_9:
                    return (uint32)EKeyboardKey::Num9;

                case GLFW_KEY_A:
                    return (uint32)EKeyboardKey::A;
                case GLFW_KEY_B:
                    return (uint32)EKeyboardKey::B;
                case GLFW_KEY_C:
                    return (uint32)EKeyboardKey::C;
                case GLFW_KEY_D:
                    return (uint32)EKeyboardKey::D;
                case GLFW_KEY_E:
                    return (uint32)EKeyboardKey::E;
                case GLFW_KEY_F:
                    return (uint32)EKeyboardKey::F;
                case GLFW_KEY_G:
                    return (uint32)EKeyboardKey::G;
                case GLFW_KEY_H:
                    return (uint32)EKeyboardKey::H;
                case GLFW_KEY_I:
                    return (uint32)EKeyboardKey::I;
                case GLFW_KEY_J:
                    return (uint32)EKeyboardKey::J;
                case GLFW_KEY_K:
                    return (uint32)EKeyboardKey::K;
                case GLFW_KEY_L:
                    return (uint32)EKeyboardKey::L;
                case GLFW_KEY_M:
                    return (uint32)EKeyboardKey::M;
                case GLFW_KEY_N:
                    return (uint32)EKeyboardKey::N;
                case GLFW_KEY_O:
                    return (uint32)EKeyboardKey::O;
                case GLFW_KEY_P:
                    return (uint32)EKeyboardKey::P;
                case GLFW_KEY_Q:
                    return (uint32)EKeyboardKey::Q;
                case GLFW_KEY_R:
                    return (uint32)EKeyboardKey::R;
                case GLFW_KEY_S:
                    return (uint32)EKeyboardKey::S;
                case GLFW_KEY_T:
                    return (uint32)EKeyboardKey::T;
                case GLFW_KEY_U:
                    return (uint32)EKeyboardKey::U;
                case GLFW_KEY_V:
                    return (uint32)EKeyboardKey::V;
                case GLFW_KEY_W:
                    return (uint32)EKeyboardKey::W;
                case GLFW_KEY_X:
                    return (uint32)EKeyboardKey::X;
                case GLFW_KEY_Y:
                    return (uint32)EKeyboardKey::Y;
                case GLFW_KEY_Z:
                    return (uint32)EKeyboardKey::Z;

                case GLFW_KEY_F1:
                    return (uint32)EKeyboardKey::F1;
                case GLFW_KEY_F2:
                    return (uint32)EKeyboardKey::F2;
                case GLFW_KEY_F3:
                    return (uint32)EKeyboardKey::F3;
                case GLFW_KEY_F4:
                    return (uint32)EKeyboardKey::F4;
                case GLFW_KEY_F5:
                    return (uint32)EKeyboardKey::F5;
                case GLFW_KEY_F6:
                    return (uint32)EKeyboardKey::F6;
                case GLFW_KEY_F7:
                    return (uint32)EKeyboardKey::F7;
                case GLFW_KEY_F8:
                    return (uint32)EKeyboardKey::F8;
                case GLFW_KEY_F9:
                    return (uint32)EKeyboardKey::F9;
                case GLFW_KEY_F10:
                    return (uint32)EKeyboardKey::F10;
                case GLFW_KEY_F11:
                    return (uint32)EKeyboardKey::F11;
                case GLFW_KEY_F12:
                    return (uint32)EKeyboardKey::F12;

                case GLFW_KEY_ESCAPE:
                    return (uint32)EKeyboardKey::Escape;
                case GLFW_KEY_ENTER:
                    return (uint32)EKeyboardKey::Enter;
                case GLFW_KEY_TAB:
                    return (uint32)EKeyboardKey::Tab;
                case GLFW_KEY_BACKSPACE:
                    return (uint32)EKeyboardKey::Backspace;
                case GLFW_KEY_INSERT:
                    return (uint32)EKeyboardKey::Insert;
                case GLFW_KEY_DELETE:
                    return (uint32)EKeyboardKey::Delete;
                case GLFW_KEY_RIGHT:
                    return (uint32)EKeyboardKey::Right;
                case GLFW_KEY_LEFT:
                    return (uint32)EKeyboardKey::Left;
                case GLFW_KEY_DOWN:
                    return (uint32)EKeyboardKey::Down;
                case GLFW_KEY_UP:
                    return (uint32)EKeyboardKey::Up;
                case GLFW_KEY_PAGE_UP:
                    return (uint32)EKeyboardKey::PageUp;
                case GLFW_KEY_PAGE_DOWN:
                    return (uint32)EKeyboardKey::PageDown;

                case GLFW_KEY_CAPS_LOCK:
                    return (uint32)EKeyboardKey::CapsLock;
                case GLFW_KEY_SCROLL_LOCK:
                    return (uint32)EKeyboardKey::ScrollLock;
                case GLFW_KEY_NUM_LOCK:
                    return (uint32)EKeyboardKey::NumLock;
                case GLFW_KEY_PRINT_SCREEN:
                    return (uint32)EKeyboardKey::PrintScreen;
                case GLFW_KEY_PAUSE:
                    return (uint32)EKeyboardKey::Pause;

                case GLFW_KEY_LEFT_SHIFT:
                    return (uint32)EKeyboardKey::LeftShift;
                case GLFW_KEY_LEFT_CONTROL:
                    return (uint32)EKeyboardKey::LeftControl;
                case GLFW_KEY_LEFT_ALT:
                    return (uint32)EKeyboardKey::LeftAlt;
                case GLFW_KEY_LEFT_SUPER:
                    return (uint32)EKeyboardKey::LeftSuper;
                case GLFW_KEY_RIGHT_SHIFT:
                    return (uint32)EKeyboardKey::RightShift;
                case GLFW_KEY_RIGHT_CONTROL:
                    return (uint32)EKeyboardKey::RightControl;
                case GLFW_KEY_RIGHT_ALT:
                    return (uint32)EKeyboardKey::RightAlt;
                case GLFW_KEY_RIGHT_SUPER:
                    return (uint32)EKeyboardKey::RightSuper;

                case GLFW_KEY_HOME:
                    return (uint32)EKeyboardKey::Home;
                case GLFW_KEY_END:
                    return (uint32)EKeyboardKey::End;
                case GLFW_KEY_MENU:
                    return (uint32)EKeyboardKey::Menu;

                default:
                    return UNDEFINED_ELEMENT;
            }
        }

    private:
        /** Primary application window (currently only single window support) */
        GLFWwindow* mWindowHandle = nullptr;

        static uint32 mReadIndex;
        static uint32 mWriteIndex;
        static InputState mStates[INPUT_STATES_COUNT];
    };

    uint32 GlfwInput::mReadIndex = 0;
    uint32 GlfwInput::mWriteIndex = 1;
    GlfwInput::InputState GlfwInput::mStates[GlfwInput::INPUT_STATES_COUNT] = {};
}

#endif //BERSERK_GLFWINPUT_H