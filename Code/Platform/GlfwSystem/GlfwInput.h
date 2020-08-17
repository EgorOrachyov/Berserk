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
#include <GlfwSystem/GlfwMouse.h>
#include <GlfwSystem/GlfwKeyboard.h>
#include <GlfwSystem/GlfwJoystick.h>
#include <GlfwSystem/GlfwWindowManager.h>

namespace Berserk {

    class GlfwInput : public Input {
    public:

        GlfwInput();
        ~GlfwInput() override = default;

        /**
         * Initialize actual input system with input events handling.
         * Called after primary application window created.
         */
        void initialize();
        void subscribeWindow(GLFWwindow* window);

        void checkConnectedJoysticks();
        void preUpdate();
        void postUpdate();
        void dispatchEvents();
        void joystickConnected();
        void joystickDisconnected();
        void processDrop(int32 count, const char** paths);

        void addMouseListener(InputListenerMouse &listener) override;
        void removeMouseListener(InputListenerMouse &listener) override;
        void addKeyboardListener(InputListenerKeyboard &listener) override;
        void removeKeyboardListener(InputListenerKeyboard &listener) override;
        void addJoystickListener(InputListenerJoystick &listener) override;
        void removeJoystickListener(InputListenerJoystick &listener) override;
        void addDropListener(InputListenerDrop &listener) override;
        void removeDropListener(InputListenerDrop &listener) override;

        ModifiersMask getModifiersMask() const override;
        Point2i getMousePosition() const override;
        Point2i getMouseDelta() const override;
        bool isMouseMoved() const override;
        bool isButtonPressed(EMouseButton button) const override;
        bool isButtonReleased(EMouseButton button) const override;

        bool isKeyPressed(EKeyboardKey key) const override;
        bool isKeyReleased(EKeyboardKey key) const override;
        bool isKeyRepeated(EKeyboardKey key) const override;

        bool hasDropInput() const override;
        void getDropInput(TArray<CString> &drop) const override;

        TPtrShared<Mouse> getMouse() const override;
        TPtrShared<Keyboard> getKeyboard() const override;

        bool hasJoysticks() const override;
        bool hasConnectedJoysticks() const override;
        uint32 getJoysticksCount() const override;
        void getJoysticks(TArray<TPtrShared<Joystick>> &joysticks) override;

        TArray<TPtrShared<GlfwJoystick>> &getGlfwJoysticks() { return mJoysticks; }
        const TPtrShared<GlfwMouse> &getGlfwMouse() { return mMouse; }
        const TPtrShared<GlfwKeyboard> &getGlfwKeyboard() { return mKeyboard; }

        static void dropCallback(GLFWwindow* window, int32 count, const char** paths);
        static void mousePositionCallback(GLFWwindow* window, double x, double y);
        static void mouseButtonsCallback(GLFWwindow* window, int32 button, int32 action, int32 mods);
        static void keyboardKeysCallback(GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods);
        static void keyboardTextCallback(GLFWwindow* window, unsigned int codePoint);
        static void joystickCallback(int32 joystickGlfwID, int32 state);

        static GlfwInput& getGlfwInput();

    private:

        int32 mConnectedJoysticksCount = 0;
        InputEventDrop mDropInput;
        TPtrShared<GlfwMouse> mMouse;
        TPtrShared<GlfwKeyboard> mKeyboard;
        TArray<TPtrShared<GlfwJoystick>> mJoysticks;
        TArray<InputListenerDrop*> mDropListeners;
        TArray<InputListenerMouse*> mMouseListeners;
        TArray<InputListenerKeyboard*> mKeyboardListeners;
        TArray<InputListenerJoystick*> mJoystickListeners;

    };

}

#endif //BERSERK_GLFWINPUT_H