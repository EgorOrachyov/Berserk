/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <GlfwSystem/GlfwInput.h>

namespace Berserk {

    GlfwInput::GlfwInput() : Input() {
        mMouse = TPtrShared<GlfwMouse>::make();
        mKeyboard = TPtrShared<GlfwKeyboard>::make();
    }

    void GlfwInput::initialize(GLFWwindow *primaryWindow) {
        subscribeWindow(primaryWindow);
        checkConnectedJoysticks();
    }

    void GlfwInput::subscribeWindow(GLFWwindow *window) {
        glfwSetDropCallback(window, dropCallback);
        glfwSetCursorPosCallback(window, mousePositionCallback);
        glfwSetMouseButtonCallback(window, mouseButtonsCallback);
        glfwSetKeyCallback(window, keyboardKeysCallback);
        glfwSetCharCallback(window, keyboardTextCallback);
        glfwSetJoystickCallback(joystickCallback);
    }

    void GlfwInput::checkConnectedJoysticks() {
        for (int32 i = 0; i < GLFW_JOYSTICK_LAST; i++) {
            if (glfwJoystickPresent(i)) {
                auto joystick = TPtrShared<GlfwJoystick>::make(i);
                mJoysticks.add(joystick);
            }
        }
    }

    void GlfwInput::preUpdate() {
        mDropInput.values.clear();
        mMouse->updateState();
        mKeyboard->updateState();
    }

    void GlfwInput::postUpdate() {
        for (auto& joystick: mJoysticks) {
            if (joystick->isConnected()) {
                joystick->updateState();
            }
        }

        dispatchEvents();
    }

    void GlfwInput::dispatchEvents() {
        auto& mouse = *mMouse;
        auto& keyboard = *mKeyboard;
        auto modifiers = keyboard.getModifiersMask();

        if (mouse.isMoved()) {
            InputEventMouse event;
            event.delta = mouse.getPositionDelta();
            event.position = mouse.getPosition();
            event.modifiersMask = modifiers;
            event.inputAction = EInputAction::Move;

            for (auto listener: mMouseListeners) {
                if (listener->onMouseEvent(event)) break;
            }
        }

        if (mouse.hasButtonInput()) {
            for (uint32 i = 0; i < Mouse::MAX_BUTTONS_COUNT; i++) {
                auto button = (EMouseButton)i;
                auto action = mouse.getButtonAction(button);

                if (action != EInputAction::Unknown) {
                    InputEventMouse event;
                    event.inputAction = action;
                    event.mouseButton = button;
                    event.modifiersMask = modifiers;

                    for (auto listener: mMouseListeners) {
                        if (listener->onMouseEvent(event)) break;
                    }
                }
            }
        }

        if (keyboard.hasKeyInput()) {
            for (uint32 i = 0; i < Keyboard::MAX_KEYS_COUNT; i++) {
                auto key = (EKeyboardKey)i;
                auto action = keyboard.getKeyAction(key);

                if (action != EInputAction::Unknown) {
                    InputEventKeyboard event;
                    event.inputAction = action;
                    event.keyboardKey = key;
                    event.modifiersMask = modifiers;

                    for (auto listener: mKeyboardListeners) {
                        if (listener->onKeyboardEvent(event)) break;
                    }
                }
            }
        }

        if (keyboard.hasTextInput()) {
            InputEventKeyboard event;
            event.inputAction = EInputAction::Text;
            event.modifiersMask = modifiers;

            for (auto codePoint: keyboard.getGlfwCodePoints()) {
                event.codepoint = codePoint;

                for (auto listener: mKeyboardListeners) {
                    if (listener->onKeyboardEvent(event)) break;
                }
            }
        }

        if (hasConnectedJoysticks()) {
            for (auto& joystickRef: mJoysticks) {
                if (joystickRef->isConnected()) {
                    auto& joystick = *joystickRef;

                    // Axis input always present, since axis shows continuous
                    // values, even if no axis is triggered by user
                    {
                        InputEventJoystick event;
                        event.inputAction = EInputAction::Move;
                        event.joystick = joystickRef.castTo<Joystick>();

                        for (uint32 i = 0; i < joystick.getAxesCount(); i++) {
                            event.axis = (EJoystickAxis)((uint32)EJoystickAxis::Axis0 + i);
                            event.value = joystick.getAxisValue(event.axis);

                            for (auto& listener: mJoystickListeners) {
                                if (listener->onJoystickEvent(event)) break;
                            }
                        }
                    }

                    if (joystick.hasButtonInput()) {
                        InputEventJoystick event;
                        event.joystick = joystickRef.castTo<Joystick>();

                        for (uint32 i = 0; i < joystick.getButtonsCount(); i++) {
                            auto button = (EJoystickButton)((uint32)EJoystickButton::Button0 + i);
                            auto action = joystick.getButtonAction(event.button);

                            if (action != EInputAction::Unknown) {
                                event.button = button;
                                event.inputAction = action;

                                for (auto& listener: mJoystickListeners) {
                                    if (listener->onJoystickEvent(event)) break;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (hasDropInput()) {
            for (auto& listener: mDropListeners) {
                if (listener->onDropInput(mDropInput)) break;
            }
        }
    }

    void GlfwInput::joystickConnected() {
        mConnectedJoysticksCount += 1;
    }

    void GlfwInput::joystickDisconnected() {
        mConnectedJoysticksCount = mConnectedJoysticksCount > 0 ? mConnectedJoysticksCount - 1: 0;
    }

    void GlfwInput::processDrop(Berserk::int32 count, const char **paths) {
        for (int32 i = 0; i < count; i++) {
            mDropInput.values.emplace(paths[i]);
        }
    }

    void GlfwInput::addMouseListener(InputListenerMouse &listener) {
        auto ptr = &listener;

        if (mMouseListeners.contains(ptr))
        BERSERK_ERROR_RET("This listener already subscribed to the input");

        mMouseListeners.add(ptr);
    }

    void GlfwInput::removeMouseListener(InputListenerMouse &listener) {
        auto ptr = &listener;
        mMouseListeners.removeElementUnordered(ptr);
    }

    void GlfwInput::addKeyboardListener(InputListenerKeyboard &listener) {
        auto ptr = &listener;

        if (mKeyboardListeners.contains(ptr))
        BERSERK_ERROR_RET("This listener already subscribed to the input");

        mKeyboardListeners.add(ptr);
    }

    void GlfwInput::removeKeyboardListener(InputListenerKeyboard &listener) {
        auto ptr = &listener;
        mKeyboardListeners.removeElementUnordered(ptr);
    }

    void GlfwInput::addJoystickListener(InputListenerJoystick &listener) {
        auto ptr = &listener;

        if (mJoystickListeners.contains(ptr))
        BERSERK_ERROR_RET("This listener already subscribed to the input");

        mJoystickListeners.add(ptr);
    }

    void GlfwInput::removeJoystickListener(InputListenerJoystick &listener) {
        auto ptr = &listener;
        mJoystickListeners.removeElementUnordered(ptr);
    }

    void GlfwInput::addDropListener(InputListenerDrop &listener) {
        auto ptr = &listener;

        if (mDropListeners.contains(ptr))
            BERSERK_ERROR_RET("This listener already subscribed to the input");

        mDropListeners.add(ptr);
    }

    void GlfwInput::removeDropListener(InputListenerDrop &listener) {
        auto ptr = &listener;
        mDropListeners.removeElementUnordered(ptr);
    }

    ModifiersMask GlfwInput::getModifiersMask() const {
        return mKeyboard->getModifiersMask();
    }

    Point2i GlfwInput::getMousePosition() const {
        return mMouse->getPosition();
    }

    Point2i GlfwInput::getMouseDelta() const {
        return mMouse->getPositionDelta();
    }

    bool GlfwInput::isMouseMoved() const {
        return mMouse->isMoved();
    }

    bool GlfwInput::isButtonPressed(EMouseButton button) const {
        return mMouse->getButtonAction(button) == EInputAction::Press;
    }

    bool GlfwInput::isButtonReleased(EMouseButton button) const {
        return mMouse->getButtonAction(button) == EInputAction::Release;
    }

    bool GlfwInput::isKeyPressed(EKeyboardKey key) const {
        return mKeyboard->getKeyAction(key) == EInputAction::Press;
    }

    bool GlfwInput::isKeyReleased(EKeyboardKey key) const {
        return mKeyboard->getKeyAction(key) == EInputAction::Release;
    }

    bool GlfwInput::isKeyRepeated(EKeyboardKey key) const {
        return mKeyboard->getKeyAction(key) == EInputAction::Repeat;
    }

    bool GlfwInput::hasDropInput() const {
        return !mDropInput.values.isEmpty();
    }

    void GlfwInput::getDropInput(TArray<CString> &drop) const {
        drop = mDropInput.values;
    }

    TPtrShared<Mouse> GlfwInput::getMouse() const {
        return mMouse.castTo<Mouse>();
    }

    TPtrShared<Keyboard> GlfwInput::getKeyboard() const {
        return mKeyboard.castTo<Keyboard>();
    }

    bool GlfwInput::hasJoysticks() const {
        return !mJoysticks.isEmpty();
    }

    bool GlfwInput::hasConnectedJoysticks() const {
        return mConnectedJoysticksCount > 0;
    }

    uint32 GlfwInput::getJoysticksCount() const {
        return mJoysticks.size();
    }

    void GlfwInput::getJoysticks(TArray<TPtrShared<Joystick>> joysticks) {
        joysticks.ensureToAdd(mJoysticks.size());

        for (auto& joystick: mJoysticks) {
            joysticks.add(joystick.castTo<Joystick>());
        }
    }

    void GlfwInput::dropCallback(GLFWwindow* window, int32 count, const char** paths) {
        auto& input = getGlfwInput();
        input.processDrop(count, paths);
    }

    void GlfwInput::mousePositionCallback(GLFWwindow* hnd, double x, double y) {
        auto& input = getGlfwInput();
        auto& mouse = input.getGlfwMouse();
        auto& manager = GlfwWindowManager::getGlfwManager();
        auto window = manager.findWindowByHandle(hnd);

        if (window.isNotNull()) {
            // Have to upscale positions since it could be retina/hdpi display
            float scaleX = window->getScaleX();
            float scaleY = window->getScaleY();

            mouse->processMovement(Point2i((int32)(x * scaleX), (int32)(y * scaleY)));
        }
    }

    void GlfwInput::mouseButtonsCallback(GLFWwindow* window, int32 button, int32 action, int32 mods) {
        auto& input = getGlfwInput();
        auto& mouse = input.getGlfwMouse();
        auto& keyboard = input.getGlfwKeyboard();

        ModifiersMask mask = GlfwDefinitions::getModsMask(mods);
        auto mouseButton = GlfwDefinitions::getMouseButton(button);
        auto mouseButtonAction = GlfwDefinitions::getAction(action);

        if (mouseButton != EMouseButton::Unknown && mouseButtonAction != EInputAction::Unknown) {
            mouse->processButtonClick(mouseButton, mouseButtonAction);
            keyboard->processModifiersUpdate(mask);
        }
    }

    void GlfwInput::keyboardKeysCallback(GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods) {
        auto& input = getGlfwInput();
        auto& keyboard = input.getGlfwKeyboard();

        ModifiersMask mask = GlfwDefinitions::getModsMask(mods);
        auto keyboardKey = GlfwDefinitions::getKeyboardKey(key);
        auto keyboardKeyAction = GlfwDefinitions::getAction(action);

        if (keyboardKey != EKeyboardKey::Unknown && keyboardKeyAction != EInputAction::Unknown) {
            keyboard->processKeyAction(keyboardKey, keyboardKeyAction, mask);
        }
    }

    void GlfwInput::keyboardTextCallback(GLFWwindow* window, unsigned int codePoint) {
        auto& input = getGlfwInput();
        auto& keyboard = input.getGlfwKeyboard();

        keyboard->processTextInput(codePoint);
    }

    void GlfwInput::joystickCallback(Berserk::int32 joystickGlfwID, Berserk::int32 state) {
        auto& input = getGlfwInput();
        auto& joysticks = input.getGlfwJoysticks();

        if (state == GLFW_CONNECTED) {
            for (auto& joystick: joysticks) {
                if (joystick->getGlfwID() == joystickGlfwID) {
                    joystick->processConnect();
                    joystick->resetState();
                    input.joystickConnected();
                    return;
                }
            }

            auto joystick = TPtrShared<GlfwJoystick>::make(joystickGlfwID);
            joystick->processConnect();
            joysticks.move(joystick);
            input.joystickConnected();
        }
        else if (state == GLFW_DISCONNECTED) {
            for (auto& joystick: joysticks) {
                if (joystick->getGlfwID() == joystickGlfwID) {
                    joystick->processDisconnect();
                    input.joystickDisconnected();
                }
            }
        }
    }

    GlfwInput& GlfwInput::getGlfwInput() {
        static auto& input = (GlfwInput&)getSingleton();
        return input;
    }

}