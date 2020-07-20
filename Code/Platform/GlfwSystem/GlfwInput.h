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
#include <GlfwSystem/GlfwWindowManager.h>
#include <GLFW/glfw3.h>

namespace Berserk {

    class GlfwInput : public Input {
    public:

        static const uint32 MOUSE_BUTTONS_COUNT = 2;
        static const uint32 KEYBOARD_KEYS_COUNT = 100;

        struct JoystickState {
            JoystickState(int32 glfwId, uint32 engineId)
                : idGLFW(glfwId),
                  id(engineId) {

                glfwGetJoystickAxes(idGLFW, &axisCount);
                glfwGetJoystickButtons(idGLFW, &buttonsCount);

                axles.resize(axisCount, 0.0f);
                buttons.resize(buttonsCount, EInputAction::Unknown);
                buttonsState.resize(buttonsCount, EInputAction::Release);

                name = glfwGetJoystickName(idGLFW);
                guid = glfwGetJoystickGUID(idGLFW);
            }

            void queryInput() {
                joystickEvent = false;

                auto pAxles = glfwGetJoystickAxes(idGLFW, &axisCount);

                for (int32 i = 0; i < axisCount; i++) {
                    axles[i] = pAxles[i];
                }

                auto pButtons = glfwGetJoystickButtons(idGLFW, &buttonsCount);

                for (int32 i = 0; i < buttonsCount; i++) {
                    auto prev = buttonsState[i];
                    auto current = getAction(pButtons[i]);
                    auto action = EInputAction::Unknown;

                    if (prev == current) {
                        action = EInputAction::Unknown;
                    }
                    else {
                        action = current;
                        joystickEvent = true;
                    }

                    buttons[i] = action;
                    buttonsState[i] = current;
                }
            }

            void resetStateData() {
                for (auto& b: buttons) {
                    b = EInputAction::Unknown;
                }
                for (auto& b: buttonsState) {
                    b = EInputAction::Release;
                }
            }

            CString               name;
            CString               guid;
            uint32                id = 0;
            int32                 idGLFW = -1;
            bool                  active = true;
            bool                  joystickEvent = false;
            int32                 axisCount = 0;
            int32                 buttonsCount = 0;
            TArray<float>         axles;
            TArray<EInputAction>  buttons;
            TArray<EInputAction>  buttonsState;
        };

        struct InputState {
            InputState() {
                resetStateData();
            }

            void queryInput() {
                for (auto &j: joysticksStates) {
                    if (j.active) {
                        j.queryInput();
                    }
                }
            }

            void resetStateData() {
                for (auto& action: mouseButtons) {
                    action = EInputAction::Unknown;
                }

                for (auto& action: keyboardKeys) {
                    action = EInputAction::Unknown;
                }

                drop.clear();
                codepoints.clear();
                mouseDelta.zero();
                modifiersMask = 0x0;
                mouseMoved = false;
                mouseEvent = false;
                keyboardEvent = false;
                keyboardTextEvent = false;
                dropEvent = false;
            }

            void updateStateData() {
                mouseEvent = false;
                keyboardEvent = false;

                for (auto& action: mouseButtons) {
                    if (action == EInputAction::Press)
                        action = EInputAction::Repeat;

                    if (action == EInputAction::Repeat)
                        mouseEvent = true;
                    else
                        action = EInputAction::Unknown;
                }

                for (auto& action: keyboardKeys) {
                    if (action == EInputAction::Press)
                        action = EInputAction::Repeat;

                    if (action == EInputAction::Repeat)
                        keyboardEvent = true;
                    else
                        action = EInputAction::Unknown;
                }

                drop.clear();
                codepoints.clear();
                mouseDelta.zero();
                modifiersMask = 0x0;
                mouseMoved = false;
                keyboardTextEvent = false;
                dropEvent = false;
            }

            void joystickConnected() {
                activeJoysticksCount += 1;
            }

            void joystickDisconnected() {
                activeJoysticksCount -= 1;
            }

            bool hasConnectedJoysticks() const {
                return activeJoysticksCount > 0;
            }

            EModifiersMask        modifiersMask = 0x0;
            Point2i               mousePosition;
            Point2i               mouseDelta;
            bool                  mouseMoved = false;
            bool                  mouseEvent = false;
            bool                  keyboardEvent = false;
            bool                  keyboardTextEvent = false;
            bool                  dropEvent = false;
            EInputAction          mouseButtons[MOUSE_BUTTONS_COUNT] = {};
            EInputAction          keyboardKeys[KEYBOARD_KEYS_COUNT] = {};
            TArray<uint32>        codepoints;
            TArray<JoystickState> joysticksStates;
            int32                 activeJoysticksCount = 0;
            TArray<CString>       drop;

        };

        GlfwInput() noexcept : Input() {}
        ~GlfwInput() override = default;

        /**
         * Initialize actual input system with input events handling.
         * Called after primary application window created.
         * @param primaryWindow Main application window to track input
         */
        void initialize(GLFWwindow* primaryWindow) {
            mWindowHandle = primaryWindow;
            glfwSetDropCallback(mWindowHandle, dropCallback);
            glfwSetCursorPosCallback(mWindowHandle, mousePositionCallback);
            glfwSetMouseButtonCallback(mWindowHandle, mouseButtonsCallback);
            glfwSetKeyCallback(mWindowHandle, keyboardKeysCallback);
            glfwSetCharCallback(mWindowHandle, keyboardTextCallback);
            glfwSetJoystickCallback(joystickCallback);
            checkConnectedJoysticks();
        }

        void reset() {
            // Note: here glfwPollEvents() not yet called
            auto& write = mState;
            write.updateStateData();
        }

        void update() {
            // Note: here glfwPollEvents() already called
            mState.queryInput();
            dispatchEvents();
        }

        void finalize() {
            mState.joysticksStates.clear();
        }

        /** All listeners receives their events (called on game thread) */
        void dispatchEvents() {
            const auto& read = mState;

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

            if (read.keyboardTextEvent) {
                for (auto codepoint: read.codepoints) {
                    InputEventKeyboard event;
                    event.inputAction = EInputAction::Text;
                    event.modifiersMask = read.modifiersMask;
                    event.codepoint = codepoint;

                    for (auto listener: mKeyboardListeners) {
                        if (listener->onKeyboardEvent(event)) break;
                    }
                }
            }

            if (read.hasConnectedJoysticks()) {
                for (auto& joystick: read.joysticksStates) {
                    if (joystick.active) {
                        {
                            InputEventJoystick event;
                            event.inputAction = EInputAction::Move;
                            event.id = joystick.id;

                            for (uint32 i = 0; i < joystick.axles.size(); i++) {
                                event.axis = (EJoystickAxis)((uint32)EJoystickAxis::Axis0 + i);
                                event.value = joystick.axles[i];

                                for (auto& listener: mJoystickListeners) {
                                    if (listener->onJoystickEvent(event)) break;
                                }
                            }
                        }

                        if (joystick.joystickEvent) {
                            InputEventJoystick event;
                            event.id = joystick.id;

                            for (uint32 i = 0; i < joystick.buttons.size(); i++) {
                                auto action = joystick.buttons[i];

                                if (action == EInputAction::Press || action == EInputAction::Release) {
                                    event.inputAction = action;
                                    event.button = (EJoystickButton)((uint32)EJoystickButton::Button0 + i);

                                    for (auto& listener: mJoystickListeners) {
                                        if (listener->onJoystickEvent(event)) break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        void addMouseListener(InputListenerMouse &listener) override {
            auto ptr = &listener;

            if (mMouseListeners.contains(ptr))
                BERSERK_ERROR_RET("This listener already subscribed to the input");

            mMouseListeners.add(ptr);
        }

        void removeMouseListener(InputListenerMouse &listener) override {
            auto ptr = &listener;
            mMouseListeners.removeElementUnordered(ptr);
        }

        void addKeyboardListener(InputListenerKeyboard &listener) override {
            auto ptr = &listener;

            if (mKeyboardListeners.contains(ptr))
                BERSERK_ERROR_RET("This listener already subscribed to the input");

            mKeyboardListeners.add(ptr);
        }

        void removeKeyboardListener(InputListenerKeyboard &listener) override {
            auto ptr = &listener;
            mKeyboardListeners.removeElementUnordered(ptr);
        }

        void addJoystickListener(InputListenerJoystick &listener) override {
            auto ptr = &listener;

            if (mJoystickListeners.contains(ptr))
                BERSERK_ERROR_RET("This listener already subscribed to the input");

            mJoystickListeners.add(ptr);
        }

        void removeJoystickListener(InputListenerJoystick &listener) override {
            auto ptr = &listener;
            mJoystickListeners.removeElementUnordered(ptr);
        }

        Point2i getMousePosition() const override {
            return mState.mousePosition;
        }

        Point2i getMouseDelta() const override {
            return mState.mouseDelta;
        }

        bool isMouseMoved() const override {
            return mState.mouseMoved;
        }

        bool isButtonPressed(EMouseButton button) const override {
            return mState.mouseButtons[(uint32)button] == EInputAction::Press;
        }

        bool isButtonReleased(EMouseButton button) const override {
            return mState.mouseButtons[(uint32)button] == EInputAction::Release;
        }

        EModifiersMask getModifiersMask() const override {
            return mState.modifiersMask;
        }

        bool isKeyPressed(EKeyboardKey key) const override {
            return mState.keyboardKeys[(uint32)key] == EInputAction::Press;
        }

        bool isKeyReleased(EKeyboardKey key) const override {
            return mState.keyboardKeys[(uint32)key] == EInputAction::Release;
        }

        bool isKeyRepeated(EKeyboardKey key) const override {
            return mState.keyboardKeys[(uint32)key] == EInputAction::Repeat;
        }

        bool isConnected(JOYSTICK_ID joystickId) const override {
            auto* j = getJoystick(joystickId);
            return j != nullptr && j->active;
        }

        void getJoysticksIds(TArray<JOYSTICK_ID> &joysticks) const override {
            joysticks.ensureToAdd(mState.joysticksStates.size());

            for (auto& j: mState.joysticksStates) {
                joysticks.add(j.id);
            }
        }

        float getJoystickAxis(JOYSTICK_ID joystickId, EJoystickAxis axis) const override {
            auto num = (uint32)axis;
            auto* j = getJoystick(joystickId);

            if (j != nullptr && j->axisCount > num) {
                return j->axles[num];
            }

            return 0.0f;
        }

        EInputAction getJoystickButton(JOYSTICK_ID joystickId, EJoystickButton button) const override {
            auto num = (uint32)button;
            auto* j = getJoystick(joystickId);

            if (j != nullptr && j->buttonsCount > num) {
                return j->buttons[num];
            }

            return EInputAction::Unknown;
        }

        bool hasDropInput() const override {
            return mState.dropEvent;
        }

        void getDropInput(TArray<CString> &drop) override {
            drop.ensureToAdd(mState.drop.size());
            drop.add(mState.drop);
        }

        static void dropCallback(GLFWwindow* window, int32 count, const char** paths)
        {
            mState.dropEvent = true;
            mState.drop.ensureToAdd(count);

            for (int32 i = 0; i < count; i++) {
                mState.drop.emplace(paths[i]);
            }
        }

        static void mousePositionCallback(GLFWwindow* window, double x, double y) {
            auto& write = mState;
            auto oldPosition = write.mousePosition;

            auto& manager = GlfwWindowManager::getGlfwManager();
            auto w = manager.findWindowByHandle(window);

            if (w.isNotNull()) {
                // Have to upscale positions since it could be retina display
                auto scaleX = w->getScaleX();
                auto scaleY = w->getScaleY();

                write.mousePosition = Point2i(static_cast<int32>(x * scaleX), static_cast<int32>(y * scaleY));
                write.mouseDelta = write.mousePosition - oldPosition;
                write.mouseMoved = true;
                write.mouseEvent = true;
            }
        }

        static void mouseButtonsCallback(GLFWwindow* window, int32 button, int32 action, int32 mods) {
            auto& write = mState;

            write.modifiersMask |= getModsMask(mods);
            auto mouseButton = getMouseButton(button);
            auto mouseButtonAction = getAction(action);

            if (mouseButton != EMouseButton::Unknown && mouseButtonAction != EInputAction::Unknown) {
                write.mouseButtons[(uint32)mouseButton] = mouseButtonAction;
                write.mouseEvent = true;
            }
        }

        static void keyboardKeysCallback(GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods) {
            auto& write = mState;

            write.modifiersMask |= getModsMask(mods);
            auto keyboardKey = getKeyboardKey(key);
            auto keyboardKeyAction = getAction(action);

            if (keyboardKey != EKeyboardKey::Unknown && keyboardKeyAction != EInputAction::Unknown) {
                write.keyboardKeys[(uint32) keyboardKey] = keyboardKeyAction;
                write.keyboardEvent = true;
            }
        }

        static void keyboardTextCallback(GLFWwindow* window, unsigned int codepoint) {
            auto& write = mState;

            write.keyboardTextEvent = true;
            write.codepoints.add(codepoint);
        }

        static void joystickCallback(int32 joystickGlfwId, int32 state) {
            if (state == GLFW_CONNECTED) {
                for (auto& j: mState.joysticksStates) {
                    if (j.idGLFW == joystickGlfwId) {
                        j.active = true;
                        j.resetStateData();
                        mState.joystickConnected();
                        return;
                    }
                }

                auto &j = mState.joysticksStates.emplace(joystickGlfwId, generateJoystickId());
                mState.joystickConnected();
            }
            else if (state == GLFW_DISCONNECTED) {
                for (auto& j: mState.joysticksStates) {
                    if (j.idGLFW == joystickGlfwId) {
                        j.active = false;
                        mState.joystickDisconnected();
                        return;
                    }
                }
            }
        }

        static uint32 generateJoystickId() {
            auto id = mNextJoystickId;
            mNextJoystickId += 1;
            return id;
        }

        static void checkConnectedJoysticks() {
            for (int32 i = 0; i < GLFW_JOYSTICK_LAST; i++) {
                if (glfwJoystickPresent(i)) {
                    mState.joysticksStates.emplace(i, generateJoystickId());
                    mState.joystickConnected();
                }
            }
        }

        static JoystickState* getJoystick(uint32 id) {
            for (auto& j: mState.joysticksStates) {
                if (j.id == id)
                    return &j;
            }

            return nullptr;
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
        TArray<InputListenerMouse*> mMouseListeners;
        TArray<InputListenerKeyboard*> mKeyboardListeners;
        TArray<InputListenerJoystick*> mJoystickListeners;
        static InputState mState;
        static uint32 mNextJoystickId;
    };

    uint32 GlfwInput::mNextJoystickId = 0;
    GlfwInput::InputState GlfwInput::mState;

}

#endif //BERSERK_GLFWINPUT_H