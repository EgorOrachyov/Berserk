/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLFWINPUT_H
#define BERSERK_GLFWINPUT_H

#include <Platform/IInput.h>
#include <ErrorMacro.h>
#include <GLFW/glfw3.h>
#include <TArray.h>

namespace Berserk {

    class GlfwInput : public IInput {
    public:

        static const uint32 INPUT_STATES_COUNT = 2;
        static const uint32 MOUSE_BUTTONS_COUNT = 2;
        static const uint32 KEYBOARD_KEYS_COUNT = 100;

        struct InputState {
            InputState() {
                for (auto& action: mouseButtons) {
                    action = EInputAction::Unknown;
                }

                for (auto& action: keyboardKeys) {
                    action = EInputAction::Unknown;
                }
            }

            void resetTmpData() {
                for (auto& action: mouseButtons) {
                    action = EInputAction::Unknown;
                }

                for (auto& action: keyboardKeys) {
                    action = EInputAction::Unknown;
                }

                mouseDelta.zero();
                modifiersMask = 0x0;
                mouseMoved = false;
                mouseEvent = false;
                keyboardEvent = false;
            }

            EModifiersMask modifiersMask = 0x0;
            Point2i        mousePosition;
            Point2i        mouseDelta;
            bool           mouseMoved = false;
            bool           mouseEvent = false;
            bool           keyboardEvent = false;
            EInputAction   mouseButtons[MOUSE_BUTTONS_COUNT] = {};
            EInputAction   keyboardKeys[KEYBOARD_KEYS_COUNT] = {};
        };

        GlfwInput() noexcept : IInput() {}
        ~GlfwInput() override = default;

        /**
         * Initialize actual input system with input events handling.
         * Called after primary application window created.
         * @param primaryWindow Main application window to track input
         */
        void initialize(GLFWwindow* primaryWindow) {
            mWindowHandle = primaryWindow;
            glfwSetCursorPosCallback(mWindowHandle, mousePositionCallback);
            glfwSetMouseButtonCallback(mWindowHandle, mouseButtonsCallback);
            glfwSetKeyCallback(mWindowHandle, keyboardKeysCallback);
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

            dispatchEvents();
        }

        /** All listeners receives their events (called on game thread) */
        void dispatchEvents() {
            const auto& read = mStates[mReadIndex];

            if (read.mouseMoved) {
                InputEventMouse event;
                event.delta = read.mouseDelta;
                event.position = read.mousePosition;
                event.modifiersMask = read.modifiersMask;
                event.inputAction = EInputAction::Move;

                for (auto listener: mMouseListeners) {
                    if (listener->onMouseEvent(event)) break;
                }
            }

            if (read.mouseEvent) {
                for (uint32 i = 0; i < MOUSE_BUTTONS_COUNT; i++) {
                    auto action = read.mouseButtons[i];
                    auto button = (EMouseButton)i;

                    if (action == EInputAction::Press || action == EInputAction::Release) {
                        InputEventMouse event;
                        event.inputAction = action;
                        event.mouseButton = button;
                        event.modifiersMask = read.modifiersMask;

                        for (auto listener: mMouseListeners) {
                            if (listener->onMouseEvent(event)) break;
                        }
                    }
                }
            }

            if (read.keyboardEvent) {
                for (uint32 i = 0; i < KEYBOARD_KEYS_COUNT; i++) {
                    auto action = read.keyboardKeys[i];
                    auto key = (EKeyboardKey)i;

                    if (action == EInputAction::Press || action == EInputAction::Release) {
                        InputEventKeyboard event;
                        event.inputAction = action;
                        event.keyboardKey = key;
                        event.modifiersMask = read.modifiersMask;

                        for (auto listener: mKeyboardListeners) {
                            if (listener->onKeyboardEvent(event)) break;
                        }
                    }
                }
            }
        }

        void addMouseListener(IInputListenerMouse &listener) override {
            auto ptr = &listener;

            if (mMouseListeners.contains(ptr))
                BERSERK_ERROR_RET("This listener already subscribed to the input");

            mMouseListeners.add(ptr);
        }

        void removeMouseListener(IInputListenerMouse &listener) override {
            auto ptr = &listener;
            mMouseListeners.removeElementUnordered(ptr);
        }

        void addKeyboardListener(IInputListenerKeyboard &listener) override {
            auto ptr = &listener;

            if (mKeyboardListeners.contains(ptr))
                BERSERK_ERROR_RET("This listener already subscribed to the input");

            mKeyboardListeners.add(ptr);
        }

        void removeKeyboardListener(IInputListenerKeyboard &listener) override {
            auto ptr = &listener;
            mKeyboardListeners.removeElementUnordered(ptr);
        }

        Point2i getMousePosition() const override {
            return mStates[mReadIndex].mousePosition;
        }

        Point2i getMouseDelta() const override {
            return mStates[mReadIndex].mouseDelta;
        }

        bool isMouseMoved() const override {
            return mStates[mReadIndex].mouseMoved;
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

        static void mousePositionCallback(GLFWwindow* window, float64 x, float64 y) {
            auto& write = mStates[mWriteIndex];
            auto& read = mStates[mReadIndex];

            write.mousePosition = Point2i(static_cast<int32>(x), static_cast<int32>(y));
            write.mouseDelta = write.mousePosition - read.mousePosition;
            write.mouseMoved = true;
            write.mouseEvent = true;
        }

        static void mouseButtonsCallback(GLFWwindow* window, int32 button, int32 action, int32 mods) {
            auto& write = mStates[mWriteIndex];

            write.modifiersMask |= getModsMask(mods);
            auto mouseButton = getMouseButton(button);

            if (mouseButton != EMouseButton::Unknown) {
                auto mouseButtonAction = getAction(action);
                write.mouseButtons[(uint32)mouseButton] = mouseButtonAction;
                write.mouseEvent = true;
            }
        }

        static void keyboardKeysCallback(GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods)
        {
            auto& write = mStates[mWriteIndex];

            write.modifiersMask |= getModsMask(mods);
            auto keyboardKey = getKeyboardKey(key);

            if (keyboardKey != EKeyboardKey::Unknown) {
                auto keyboardKeyAction = getAction(action);
                write.keyboardKeys[(uint32) keyboardKey] = keyboardKeyAction;
                write.keyboardEvent = true;
            }
        }

        static EModifiersMask getModsMask(int32 mods) {
            EModifiersMask mask = 0x0;
            auto umods = (uint32) mods;

            if (umods & (uint32)GLFW_MOD_ALT)
                mask |= (uint32) EModifierMask::Alt;
            if (umods & (uint32)GLFW_MOD_CAPS_LOCK)
                mask |= (uint32) EModifierMask::CapsLock;
            if (umods & (uint32)GLFW_MOD_CONTROL)
                mask |= (uint32) EModifierMask::Control;
            if (umods & (uint32)GLFW_MOD_NUM_LOCK)
                mask |= (uint32) EModifierMask::NumLock;
            if (umods & (uint32)GLFW_MOD_SHIFT)
                mask |= (uint32) EModifierMask::Shift;

            return mask;
        }

        static EMouseButton getMouseButton(int32 button) {
            switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT:
                    return EMouseButton::Left;
                case GLFW_MOUSE_BUTTON_RIGHT:
                    return EMouseButton::Right;
                default:
                    return EMouseButton::Unknown;
            }
        }

        static EInputAction getAction(int32 action) {
            switch (action) {
                case GLFW_PRESS:
                    return EInputAction::Press;
                case GLFW_RELEASE:
                    return EInputAction::Release;
                default:
                    return EInputAction::Unknown;
            }
        }

        static EKeyboardKey getKeyboardKey(int32 key) {
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
                    return EKeyboardKey::Unknown;
            }
        }

    private:
        /** Primary application window (currently only single window support) */
        GLFWwindow* mWindowHandle = nullptr;
        TArray<IInputListenerMouse*> mMouseListeners;
        TArray<IInputListenerKeyboard*> mKeyboardListeners;

        static uint32 mReadIndex;
        static uint32 mWriteIndex;
        static InputState mStates[INPUT_STATES_COUNT];
    };

    uint32 GlfwInput::mReadIndex = 0;
    uint32 GlfwInput::mWriteIndex = 1;
    GlfwInput::InputState GlfwInput::mStates[GlfwInput::INPUT_STATES_COUNT] = {};
}

#endif //BERSERK_GLFWINPUT_H